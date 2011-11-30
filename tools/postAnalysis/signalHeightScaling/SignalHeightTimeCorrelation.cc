#include "SignalHeightTimeCorrelation.hh"

#include "Corrections.hh"
#include "Constants.hh"
#include "RootQtWidget.hh"

#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TLatex.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TSpline.h>

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QDebug>
#include <QStringList>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

QMap<unsigned short, TGraph*> SignalHeightTimeCorrelation::s_factorGraphs;

SignalHeightTimeCorrelation::SignalHeightTimeCorrelation(PostAnalysisCanvas* canvas, unsigned short sipmId)
  : PostAnalysisPlot()
  , GraphPlot()
  , m_histogram(0)
  , m_sipmId(sipmId)
  , m_secondaryRootWidget(0)
  , m_spline(0)
{
  m_histogram = canvas->histograms2D().at(0);
  QString title = QString(canvas->name()).replace("canvas", "graph");
  setTitle(title);
  addGraph(meanGraph(sipmId, m_histogram), P);
  setAxisTitle("time", "signal height / adc counts");
  TLatex* latex = 0;
  latex = RootPlot::newLatex(.47, .85);
  latex->SetTitle(qPrintable(QString("sipm id = 0x%1").arg(QString::number(sipmId, 16))));
  addLatex(latex);

  m_secondaryRootWidget = new RootQtWidget;
  QWidget* widget = new QWidget;
  QVBoxLayout* layout = new QVBoxLayout(widget);
  layout->setContentsMargins(0, 0, 0, 0);
  QWidget* saveWidget = new QWidget;
  QHBoxLayout* saveLayout = new QHBoxLayout(saveWidget);
  saveLayout->setContentsMargins(0, 0, 0, 0);
  QPushButton* saveButton = new QPushButton("save channel");
  saveLayout->addWidget(saveButton);
  QPushButton* saveAllButton = new QPushButton("save all channels");
  saveLayout->addWidget(saveAllButton);
  saveLayout->addStretch();
  layout->addWidget(saveWidget);
  layout->addWidget(m_secondaryRootWidget);
  setSecondaryWidget(widget);
  connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
  connect(saveAllButton, SIGNAL(clicked()), this, SLOT(saveAll()));
  drawFactorGraph();
}

SignalHeightTimeCorrelation::~SignalHeightTimeCorrelation()
{
  delete m_spline;
}

void SignalHeightTimeCorrelation::drawFactorGraph()
{
  if (m_spline) {
    delete m_spline;
    m_spline = 0;
  }
  TVirtualPad* prevPad = gPad;
  TCanvas* can = m_secondaryRootWidget->GetCanvas();
  can->cd();
  can->Clear();
  TGraph* factorGraph = s_factorGraphs[m_sipmId];
  factorGraph->Draw("ALP");
  m_spline = new TSpline3(qPrintable(QString("spline 0x%1").arg(QString::number(m_sipmId, 16))), factorGraph);
  m_spline->SetLineColor(kRed);
  m_spline->SetLineWidth(2);
  m_spline->Draw("LSAME");
  can->Modified();
  can->Update();
  prevPad->cd();
  gPad->Modified();
  gPad->Update();
}

TGraphErrors* SignalHeightTimeCorrelation::meanGraph(unsigned short sipmId, TH2D* histogram) 
{
  const double referenceValue = Constants::idealTrackerSignalHeight;
  const double minAdc = 300;
  const int minEntries = 30;
  const int minTotalEntries = 30;
  TGraphErrors* graph = new TGraphErrors();
  TGraph* factorGraph = 0;
  if (!s_factorGraphs.keys().contains(sipmId)) {
    factorGraph = new TGraphErrors();
    s_factorGraphs.insert(sipmId, factorGraph);
  } else {
    factorGraph = s_factorGraphs[sipmId];
    factorGraph->Set(0);
  }
  double timeLow = histogram->GetXaxis()->GetBinLowEdge(1);
  double timeUp = histogram->GetXaxis()->GetBinLowEdge(histogram->GetXaxis()->GetNbins()+1);
  TH1D* totalProjection = histogram->ProjectionY("_full_py", 1, histogram->GetNbinsX());
  double totalAdc = totalProjection->GetMean();
  double totalAdcError = totalProjection->GetRMS() / sqrt(totalProjection->GetEntries());
  if (totalProjection->GetEntries() > minTotalEntries) {
    TF1* totalFunction = new TF1(qPrintable(QString(totalProjection->GetTitle()) + "Function"), "landau", totalProjection->GetXaxis()->GetXmin(), totalProjection->GetXaxis()->GetXmax());
    totalProjection->Fit(totalFunction, "EQN0");
    if (totalFunction->GetParameter(1) > 0) {
      totalAdc = totalFunction->GetParameter(1);
      totalAdcError = totalFunction->GetParError(1);
    }
    delete totalFunction;
    totalFunction = 0;
  }
  delete totalProjection;
  totalProjection = 0;
  for (int bin = 0; bin < histogram->GetNbinsX(); ++bin) {
    TH1D* projectionHistogram = histogram->ProjectionY("_py", bin + 1, bin + 1);
    int nEntries = projectionHistogram->GetEntries();
    TF1* function = new TF1(qPrintable(QString(projectionHistogram->GetTitle()) + "Function"), "landau", projectionHistogram->GetXaxis()->GetXmin(), projectionHistogram->GetXaxis()->GetXmax());
    projectionHistogram->Fit(function, "EQN0");
    double adc = function->GetParameter(1);
    double adcError = function->GetParError(1);
    delete function;
    function = 0;
    double time = histogram->GetXaxis()->GetBinCenter(bin+1);
    double timeError = histogram->GetXaxis()->GetBinWidth(bin+1) / sqrt(12);
    if (adc > minAdc && nEntries > minEntries) {
      int nPoints = graph->GetN();
      graph->SetPoint(nPoints, time, adc);
      graph->SetPointError(nPoints, timeError, adcError);
    } else {
      adc = totalAdc;
      adcError = totalAdcError;
    }
    if (factorGraph->GetN() == 0)
      factorGraph->SetPoint(factorGraph->GetN(), timeLow, referenceValue / adc);
    factorGraph->SetPoint(factorGraph->GetN(), time, referenceValue / adc);
    delete projectionHistogram;
    projectionHistogram = 0;
  }
  double x, y;
  factorGraph->GetPoint(factorGraph->GetN() - 1, x, y);
  factorGraph->SetPoint(factorGraph->GetN(), timeUp, y);
  return graph;
}

void SignalHeightTimeCorrelation::save(unsigned short sipmId) 
{
  Q_ASSERT(s_factorGraphs[sipmId]);
  TGraph* graph = s_factorGraphs[sipmId];
  QVector<double> times;
  QVector<double> factors;
  for (int i = 0; i < graph->GetN(); ++i) {
    times.push_back(graph->GetX()[i]);
    factors.push_back(graph->GetY()[i]);
  }
  Corrections correction;
  correction.writeTrackerSignalScaling(sipmId, times, factors);
}

void SignalHeightTimeCorrelation::save()
{
  save(m_sipmId);
}

void SignalHeightTimeCorrelation::saveAll()
{
  foreach (unsigned short sipmId, s_factorGraphs.keys())
    save(sipmId);
}

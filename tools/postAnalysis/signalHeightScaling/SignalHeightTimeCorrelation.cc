#include "SignalHeightTimeCorrelation.hh"

#include "Corrections.hh"
#include "Constants.hh"

#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TLatex.h>
#include <TMultiGraph.h>

#include <iostream>
#include <iomanip>
#include <math.h>

#include <QDebug>
#include <QStringList>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

QMap<unsigned short, TGraph*> SignalHeightTimeCorrelation::s_graphs;

SignalHeightTimeCorrelation::SignalHeightTimeCorrelation(PostAnalysisCanvas* canvas, unsigned short sipmId)
  : PostAnalysisPlot()
  , GraphPlot()
  , m_sipmId(sipmId)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  QString title = QString(canvas->name()).replace("canvas", "graph");
  setTitle(title);
  addGraph(meanGraph(sipmId, histogram), P);
  setAxisTitle("time", "signal height / adc counts");
  if (!s_graphs.keys().contains(sipmId))
    s_graphs.insert(sipmId, graph());
  TLatex* latex = 0;
  latex = RootPlot::newLatex(.47, .85);
  latex->SetTitle(qPrintable(QString("sipm id = 0x%1").arg(QString::number(sipmId, 16))));
  addLatex(latex);

  QWidget* widget = new QWidget;
  QHBoxLayout* layout = new QHBoxLayout(widget);
  layout->setContentsMargins(0, 0, 0, 0);
  QPushButton* saveButton = new QPushButton("save channel");
  layout->addWidget(saveButton);
  QPushButton* saveAllButton = new QPushButton("save all channels");
  layout->addWidget(saveAllButton);
  layout->addStretch();
  setSecondaryWidget(widget);
  connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
  connect(saveAllButton, SIGNAL(clicked()), this, SLOT(saveAll()));
}

SignalHeightTimeCorrelation::~SignalHeightTimeCorrelation()
{
}

TGraphErrors* SignalHeightTimeCorrelation::meanGraph(unsigned short sipmId, TH2D* histogram) 
{
  const double minAdc = 0;
  const int minEntries = 30;
  TGraphErrors* graph = new TGraphErrors();
  TGraphErrors* factorGraph = new TGraphErrors();
  s_graphs.insert(sipmId, factorGraph);
  bool isFirst = true;
  bool isLast = false;
  for (int bin = 0; bin < histogram->GetNbinsX(); ++bin) {
    TH1D* projectionHistogram = histogram->ProjectionY("_py", bin + 1, bin + 1);
    int nEntries = projectionHistogram->GetEntries();
    TF1* function = new TF1(qPrintable(QString(projectionHistogram->GetTitle()) + "Function"), "landau", projectionHistogram->GetXaxis()->GetXmin(), projectionHistogram->GetXaxis()->GetXmax());
    projectionHistogram->Fit(function, "EQN0");
    double adc = function->GetParameter(1);
    double adcError = function->GetParError(1);
    delete function;
    double time = histogram->GetXaxis()->GetBinCenter(bin+1);
    double timeError = histogram->GetXaxis()->GetBinWidth(bin+1) / sqrt(12);
    if (isFirst) {
      time = histogram->GetXaxis()->GetBinLowEdge(bin+1);
      timeError = 0;
      bin--;
      isFirst = false;
    }
    if (isLast) {
      time = histogram->GetXaxis()->GetBinLowEdge(bin+2);
      timeError = 0;
    }
    if (!isLast && bin == histogram->GetNbinsX() - 1) {
      isLast = true;
      bin--;
    }
    int nFactorPoints = factorGraph->GetN();
    if (adc > minAdc && nEntries > minEntries) {
      int nPoints = graph->GetN();
      graph->SetPoint(nPoints, time, adc);
      graph->SetPointError(nPoints, timeError, adcError);
      factorGraph->SetPoint(nPoints, time, Constants::idealTrackerSignalHeight / adc);
      factorGraph->SetPointError(nPoints, timeError, adcError);
    } else {
      factorGraph->SetPoint(nFactorPoints, time, 1);
      factorGraph->SetPointError(nFactorPoints, 0, 0);
    }
  }
  return graph;
}

void SignalHeightTimeCorrelation::save(unsigned short sipmId) 
{
  Q_ASSERT(s_graphs[sipmId]);
  TGraph* graph = s_graphs[sipmId];
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
  foreach (unsigned short sipmId, s_graphs.keys())
    save(sipmId);
}

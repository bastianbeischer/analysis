#include "SignalHeightTimeCor.hh"

#include "Corrections.hh"

#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
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

QMap<unsigned short, TF1> SignalHeightTimeCor::s_SignalHeightTimeCorFits;

SignalHeightTimeCor::SignalHeightTimeCor(PostAnalysisCanvas* canvas, unsigned short sipmId)
: PostAnalysisPlot()
, GraphPlot()
, m_sipmId(sipmId)
, m_graph(0)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  fit(sipmId, histogram);
  QString title = QString(canvas->name()).replace("canvas", "graph");
  setTitle(title);
  addGraph(new TGraphErrors(m_graph), P);
  TF1& f = s_SignalHeightTimeCorFits[sipmId];
  addFunction(&f);
  setAxisTitle("time", "signal height / adc counts");

  TLatex* latex = 0;
  latex = RootPlot::newLatex(.47, .85);
  latex->SetTitle(qPrintable(QString("sipm id = 0x%1").arg(QString::number(sipmId, 16))));
  addLatex(latex);

  latex = RootPlot::newLatex(.47, .82);
  latex->SetTitle(qPrintable(QString("offset = %1 adc counts").arg(f.GetParameter(0))));
  addLatex(latex);

  latex = RootPlot::newLatex(.47, .79);
  latex->SetTitle(qPrintable(QString("slope = %1 adc counts / #circC").arg(f.GetParameter(1))));
  addLatex(latex);

//  graph()->GetYaxis()->SetRangeUser(graph()->GetMinimum() * 0.9, graph()->GetMaximum() * 1.1);
  function()->SetRange(graph()->GetXaxis()->GetBinLowEdge(1), graph()->GetXaxis()->GetBinLowEdge(graph()->GetXaxis()->GetNbins()));

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

SignalHeightTimeCor::~SignalHeightTimeCor()
{
}

void SignalHeightTimeCor::fit(unsigned short sipmId, TH2D* histogram) 
{
  const double minAdc = 0;
  const int minEntries = 100;
  TGraphErrors graph;

  for (int bin = 0; bin < histogram->GetNbinsX(); ++bin) {
    TH1* projectionHistogram = histogram->ProjectionY("_py", bin + 1, bin + 1);
    int nEntries = projectionHistogram->GetEntries();
    TF1* function = new TF1(qPrintable(QString(histogram->GetTitle()) + "Function"), "gaus", histogram->GetXaxis()->GetXmin(), histogram->GetXaxis()->GetXmax());
    projectionHistogram->Fit(function, "EQN0");
    double mean = function->GetParameter(1);
    double sigma = projectionHistogram->GetRMS();
    delete function;

    double adc = mean;
    double adcError = sigma / sqrt(nEntries);
    double temperature = histogram->GetBinCenter(bin+1);
    double temperatureError = histogram->GetBinWidth(bin+1)/sqrt(12);

    if (adc > minAdc && nEntries > minEntries) {
      int nPoints = graph.GetN();
      graph.SetPoint(nPoints, temperature, adc);
      graph.SetPointError(nPoints, temperatureError, adcError);
    }
  }
  m_graph = graph;

  //Linear fit
  QString htitle =QString("Fit signal height time dependent 0x%1").arg(sipmId,0,16);
  TF1 f(qPrintable(htitle), "pol1");
  f.SetLineColor(kRed);
  f.SetLineWidth(1);
  f.SetLineStyle(2);
  graph.Fit(&f, "EQ");
  s_SignalHeightTimeCorFits.insert(sipmId, f);
}

void SignalHeightTimeCor::save(unsigned short sipmId) 
{
  QList<QVariant> param;
  const TF1& f = s_SignalHeightTimeCorFits[sipmId];
  param.push_back( f.GetParameter(0) );
  param.push_back( f.GetParameter(1) );
  Corrections* correction = new Corrections();
  Q_ASSERT(false);
  //todo save for tracker corrections
//  correction->setTotScaling(tofId, param);
  delete correction;
}

void SignalHeightTimeCor::save()
{
  save(m_sipmId);
}

void SignalHeightTimeCor::saveAll()
{
  foreach (unsigned int sipmId, s_SignalHeightTimeCorFits.keys()) {
    save(sipmId);
  }
}

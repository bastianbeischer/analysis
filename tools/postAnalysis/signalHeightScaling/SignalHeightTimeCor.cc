#include "SignalHeightTimeCor.hh"

#include "Corrections.hh"

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

SignalHeightTimeCor::SignalHeightTimeCor(PostAnalysisCanvas* canvas, unsigned short sipmId)
: PostAnalysisPlot()
, GraphPlot()
, m_sipmId(sipmId)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  QString title = QString(canvas->name()).replace("canvas", "graph");
  setTitle(title);
  addGraph(meanGraph(sipmId, histogram), P);
  setAxisTitle("time", "signal height / adc counts");

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

SignalHeightTimeCor::~SignalHeightTimeCor()
{
}

TGraphErrors* SignalHeightTimeCor::meanGraph(unsigned short, TH2D* histogram) 
{
  const double minAdc = 0;
  const int minEntries = 100;
  TGraphErrors* graph = new TGraphErrors();

  for (int bin = 0; bin < histogram->GetNbinsX(); ++bin) {
    TH1* projectionHistogram = histogram->ProjectionY("_py", bin + 1, bin + 1);
    int nEntries = projectionHistogram->GetEntries();
    double mean = projectionHistogram->GetMean();
    double sigma = projectionHistogram->GetRMS();

    double adc = mean;
    double adcError = sigma / sqrt(nEntries);
    double temperature = histogram->GetBinCenter(bin+1);
    double temperatureError = histogram->GetBinWidth(bin+1)/sqrt(12);

    if (adc > minAdc && nEntries > minEntries) {
      int nPoints = graph->GetN();
      graph->SetPoint(nPoints, temperature, adc);
      graph->SetPointError(nPoints, temperatureError, adcError);
    }
  }
  return graph;
}

void SignalHeightTimeCor::save(unsigned short) 
{
  Q_ASSERT(false);
  //todo save for tracker corrections
}

void SignalHeightTimeCor::save()
{
  save(m_sipmId);
}

void SignalHeightTimeCor::saveAll()
{
  Q_ASSERT(false);
  //todo save for tracker corrections
}

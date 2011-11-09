#include "SignalHeightTempCorrelation.hh"

#include "Corrections.hh"

#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TLatex.h>

#include <iostream>
#include <iomanip>
#include <math.h>

#include <QDebug>
#include <QStringList>

SignalHeightTempCorrelation::SignalHeightTempCorrelation(PostAnalysisCanvas* canvas, unsigned short sipmId)
  : PostAnalysisPlot()
  , GraphPlot()
  , m_sipmId(sipmId)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  QString title = QString(canvas->name()).replace("canvas", "graph");
  setTitle(title);
  addGraph(meanGraph(histogram), P);
  setAxisTitle("temperature /  #circC", "signal height / adc counts");

  TLatex* latex = 0;
  latex = RootPlot::newLatex(.47, .85);
  latex->SetTitle(qPrintable(QString("sipm id = 0x%1").arg(QString::number(sipmId, 16))));
  addLatex(latex);
}

SignalHeightTempCorrelation::~SignalHeightTempCorrelation()
{
}

TGraphErrors* SignalHeightTempCorrelation::meanGraph(TH2D* histogram) 
{
  const double minAdc = 0;
  const int minEntries = 10;
  TGraphErrors* graph = new TGraphErrors;

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

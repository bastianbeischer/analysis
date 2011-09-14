#include "TimeOverThresholdScaling.hh"

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

QMap<unsigned int, TGraphErrors> TimeOverThresholdScaling::timeOverThresholdScalingGraphs;
QMap<unsigned int, TF1> TimeOverThresholdScaling::timeOverThresholdScalingFits;
QMap<unsigned int, double> TimeOverThresholdScaling::minTofTemps;
QMap<unsigned int , double> TimeOverThresholdScaling::maxTofTemps;

TimeOverThresholdScaling::TimeOverThresholdScaling(PostAnalysisCanvas* canvas, unsigned int tofId)
  : PostAnalysisPlot()
  , GraphPlot()
{
  TH2D* histogram = canvas->histograms2D().at(0);
  scaling(tofId, histogram);
  QString title = QString(canvas->name()).replace("canvas", "graph");
  setTitle(title);
  TGraphErrors* graph = new TGraphErrors(TimeOverThresholdScaling::timeOverThresholdScalingGraphs[tofId]);
  addGraph(graph, P);
  TF1& f = TimeOverThresholdScaling::timeOverThresholdScalingFits[tofId];
  addFunction(&f);
  setAxisTitle("temperature /  #circC", "time over threshold / ns");

  TLatex* latex = 0;
  latex = RootPlot::newLatex(.47, .85);
  latex->SetTitle(qPrintable(QString("id = 0x%1").arg(QString::number(tofId, 16))));
  addLatex(latex);

  latex = RootPlot::newLatex(.47, .82);
  latex->SetTitle(qPrintable(QString("offset = %1 ns").arg(f.GetParameter(0))));
  addLatex(latex);

  latex = RootPlot::newLatex(.47, .79);
  latex->SetTitle(qPrintable(QString("slope = %1 ns / #circC").arg(f.GetParameter(1))));
  addLatex(latex);

  graph->GetYaxis()->SetRangeUser(15, 40);
  function()->SetRange(graph->GetXaxis()->GetBinLowEdge(1), graph->GetXaxis()->GetBinLowEdge(graph->GetXaxis()->GetNbins()));

}

TimeOverThresholdScaling::~TimeOverThresholdScaling()
{
}

void TimeOverThresholdScaling::scaling(unsigned int tofId, TH2D* histogram) 
{
  const double minTot = 0;
  const int minEntries = 100;
  TGraphErrors graph;
  
  QMap<double, double> temperatureMap;
  
  for (int bin = 0; bin < histogram->GetNbinsX(); ++bin) {
    TH1* projectionHistogram = histogram->ProjectionY("_py", bin + 1, bin + 1);
    int nEntries = projectionHistogram->GetEntries();
    TF1* function = new TF1(qPrintable(QString(histogram->GetTitle()) + "Function"), "landau", histogram->GetXaxis()->GetXmin(), histogram->GetXaxis()->GetXmax());
    projectionHistogram->Fit(function, "EQN0");
    double mpv = function->GetParameter(1);
    double sigma = projectionHistogram->GetRMS();
    delete function;

    double tot = mpv;
    double totError = sigma / sqrt(nEntries);
    double temperature = histogram->GetBinCenter(bin+1);
    double temperatureError = histogram->GetBinWidth(bin+1)/sqrt(12);
    
    if (tot > minTot && nEntries > minEntries) {
      int nPoints = graph.GetN();
      graph.SetPoint(nPoints, temperature, tot);
      graph.SetPointError(nPoints, temperatureError, totError);
      temperatureMap.insert(temperature, tot);
    }
  }
  
  double min = 0;
  double max = 0;
  
  if (temperatureMap.keys().size() > 0) {
    min = temperatureMap.keys()[0];
    max = min;
    
    foreach(double temperature, temperatureMap.keys()) {
      if (temperature < min) {
        min = temperature;
      }
      if (temperature > max) {
        max = temperature;
      }
    }
  }

  TimeOverThresholdScaling::minTofTemps.insert(tofId, min);
  TimeOverThresholdScaling::maxTofTemps.insert(tofId, max);
  
  TimeOverThresholdScaling::timeOverThresholdScalingGraphs.insert(tofId, graph);
  
  //Linear fit
  QString htitle =QString("Fit time over threshold temperature dependent 0x%1").arg(tofId,0,16);
  TF1 f(qPrintable(htitle), "pol1");
  f.SetLineColor(kRed);
  f.SetLineWidth(1);
  f.SetLineStyle(2);
  graph.Fit(&f, "EQ");
  TimeOverThresholdScaling::timeOverThresholdScalingFits.insert(tofId, f);
  
  QList<QVariant> param;
  param.push_back( f.GetParameter(0) );
  param.push_back( f.GetParameter(1) );
  
  Corrections* correction = new Corrections();
  correction->setTotScaling(tofId, param);
  delete correction;
}

#include "BetaMomentumCorrelationPlot.hh"
#include "BrokenLine.hh"
#include "Constants.hh"

#include "TrackInformation.hh"
#include "Hit.hh"
#include "TOFCluster.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>

#include <QDebug>

double betaMomentumCorrelationFunction(double x[], double p[])
{
  double rigidity = qAbs(x[0]) < 0.01 ? 0.01 : qAbs(x[0]);
  double momentum = rigidity*p[1] ;
  return sqrt(momentum*momentum+p[0]*p[0])/momentum;
}

BetaMomentumCorrelationPlot::BetaMomentumCorrelationPlot()
  : AnalysisPlot(AnalysisPlot::MomentumReconstruction)
  , H2DPlot()
{
  setTitle(QString("beta vs momentum"));
  int nBinsX = 120;
  double xMin = -6;
  double xMax = 6;
  int nBinsY = 200;
  double yMin = -10;
  double yMax = 10;
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  setHistogram(histogram);
  setAxisTitle("R / GV", "1 / #beta", "");

  TF1* function = 0;
  TLegend* legend = new TLegend(.12, .72, .23, .88);
  legend->SetMargin(.7);

  function = new TF1("electronCorrelation", betaMomentumCorrelationFunction, xMin, xMax, 2);
  function->SetParameter(0, Constants::electronMass);
  function->SetParameter(1, 1);
  function->SetNpx(1000);
  function->SetLineColor(kBlue);
  function->SetLineStyle(2);
  legend->AddEntry(function, "e^{#pm}", "l");
  addFunction(function);

  function = new TF1("muonCorrelation", betaMomentumCorrelationFunction, xMin, xMax, 2);
  function->SetParameter(0, Constants::muonMass);
  function->SetParameter(1, 1);
  function->SetNpx(1000);
  function->SetLineColor(kGreen);
  function->SetLineStyle(2);
  legend->AddEntry(function, "#mu^{#pm}", "l");
  addFunction(function);

  function = new TF1("pionCorrelation", betaMomentumCorrelationFunction, xMin, xMax, 2);
  function->SetParameter(0, Constants::pionMass);
  function->SetParameter(1, 1);
  function->SetNpx(1000);
  function->SetLineColor(kCyan);
  function->SetLineStyle(2);
  legend->AddEntry(function, "#pi^{#pm}", "l");
  addFunction(function);

  function = new TF1("protonCorrelation", betaMomentumCorrelationFunction, xMin, xMax, 2);
  function->SetParameter(0, Constants::protonMass);
  function->SetParameter(1, 1);
  function->SetLineColor(kMagenta);
  function->SetNpx(1000);
  function->SetLineStyle(2);
  legend->AddEntry(function, "p^{#pm}", "l");
  addFunction(function);

  function = new TF1("He", betaMomentumCorrelationFunction, 0.01, xMax, 2);
  function->SetParameter(0, Constants::heliumMass);
  function->SetParameter(1, 2);
  function->SetLineColor(kRed);
  function->SetNpx(1000);
  function->SetLineStyle(2);
  legend->AddEntry(function, "He", "l");
  addFunction(function);
  
  addLegend(legend);
}

BetaMomentumCorrelationPlot::~BetaMomentumCorrelationPlot()
{
}

void BetaMomentumCorrelationPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & (TrackInformation::AllTrackerLayers | TrackInformation::InsideMagnet)))
    return;
  histogram()->Fill(track->rigidity(), 1./track->beta());
}

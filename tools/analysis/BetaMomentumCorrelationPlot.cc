#include "BetaMomentumCorrelationPlot.hh"
#include "BrokenLine.hh"
#include "Constants.hh"

#include "ParticleInformation.hh"
#include "Hit.hh"
#include "TOFCluster.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Hypothesis.hh"

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
  : AnalysisPlot(Enums::MomentumReconstruction)
  , H2DPlot()
{
  setTitle(QString("beta vs momentum"));
  int nBinsX = 121;
  double xMin = -6;
  double xMax = 6;
  int nBinsY = 201;
  double binWidthX = (xMax - xMin) / nBinsX;
  double yMin = -10;
  double yMax = 10;
  double binWidthY = (yMax - yMin) / nBinsY;
  TH2D* histogram = new TH2D(qPrintable(title()), "",
    nBinsX, xMin - binWidthX / 2., xMax + binWidthX / 2.,
    nBinsY, yMin - binWidthY / 2., yMax + binWidthY / 2.);
  addHistogram(histogram);
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

void BetaMomentumCorrelationPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet | ParticleInformation::BetaGood))
    return;
  histogram()->Fill(event->particle()->hypothesis()->rigidity(), 1./event->particle()->hypothesis()->beta());
}

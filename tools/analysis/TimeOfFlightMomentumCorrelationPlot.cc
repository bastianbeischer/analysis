#include "TimeOfFlightMomentumCorrelationPlot.hh"
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

double timeOfFlightMomentumCorrelationFunction(double x[], double p[])
{
  double v = qAbs(x[0]) < 0.01 ? 0.01 : qAbs(x[0]);
  return sqrt(v*v+p[0]*p[0])/v;
}

TimeOfFlightMomentumCorrelationPlot::TimeOfFlightMomentumCorrelationPlot()
  : AnalysisPlot(AnalysisPlot::MomentumReconstruction)
  , H2DPlot()
{
  setTitle(QString("tof momentum correlation"));
  int nBinsX = 120;
  double xMin = -6;
  double xMax = 6;
  int nBinsY = 200;
  double yMin = -10;
  double yMax = 10;
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  histogram->GetXaxis()->SetTitle("p / GeV");
  histogram->GetYaxis()->SetTitle("1 / #beta");
  setHistogram(histogram);
  TF1* function = 0;
  TLegend* legend = new TLegend(.12, .72, .23, .88);
  legend->SetMargin(.7);

  function = new TF1("electronCorrelation", timeOfFlightMomentumCorrelationFunction, xMin, xMax, 1);
  function->SetParameter(0, electronMass);
  function->SetNpx(1000);
  function->SetLineColor(kBlue);
  function->SetLineStyle(2);
  legend->AddEntry(function, "e^{#pm}", "l");
  addFunction(function);

  function = new TF1("muonCorrelation", timeOfFlightMomentumCorrelationFunction, xMin, xMax, 1);
  function->SetParameter(0, muonMass);
  function->SetNpx(1000);
  function->SetLineColor(kGreen);
  function->SetLineStyle(2);
  legend->AddEntry(function, "#mu^{#pm}", "l");
  addFunction(function);

  function = new TF1("pionCorrelation", timeOfFlightMomentumCorrelationFunction, xMin, xMax, 1);
  function->SetParameter(0, pionMass);
  function->SetNpx(1000);
  function->SetLineColor(kCyan);
  function->SetLineStyle(2);
  legend->AddEntry(function, "#pi^{#pm}", "l");
  addFunction(function);

  function = new TF1("protonCorrelation", timeOfFlightMomentumCorrelationFunction, xMin, xMax, 1);
  function->SetParameter(0, protonMass);
  function->SetLineColor(kMagenta);
  function->SetNpx(1000);
  function->SetLineStyle(2);
  legend->AddEntry(function, "p^{#pm}", "l");
  addFunction(function);

  function = new TF1("He", timeOfFlightMomentumCorrelationFunction, 0.01, xMax, 1);
  function->SetParameter(0, heliumMass);
  function->SetLineColor(kRed);
  function->SetNpx(1000);
  function->SetLineStyle(2);
  legend->AddEntry(function, "He", "l");
  addFunction(function);
  
  addLegend(legend);
}

TimeOfFlightMomentumCorrelationPlot::~TimeOfFlightMomentumCorrelationPlot()
{
}

void TimeOfFlightMomentumCorrelationPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & (TrackInformation::AllTrackerLayers | TrackInformation::InsideMagnet)))
    return;
  histogram()->Fill(track->p(), 1./track->beta());
}

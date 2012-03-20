#include "AzimuthCutStatistics.hh"

#include <QDebug>

#include <TH2D.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"
#include "ProjectionControlWidget.hh"
#include "Constants.hh"

#include <QSpinBox>

#include <iostream>
#include <cmath>

AzimuthCutStatistics::AzimuthCutStatistics()
  : AnalysisPlot(Enums::Tracking)
  , H2DProjectionPlot()
{
  setTitle("Azimuth cut satistics");

  const int nBinsAzimuth = 45;
  const double minAzimuth = -180;
  const double maxAzimuth = 180;

  const int nBinsCuts = 8;
  const double minCuts = -0.5;
  const double maxCuts = 7.5;

  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsCuts, minCuts, maxCuts, nBinsAzimuth, minAzimuth, maxAzimuth);

  histogram->Sumw2();
  setAxisTitle("cut", "azimuth / degree", "");
  addHistogram(histogram);

  setDrawOption(COLZ);
  controlWidget()->spinBox()->setMaximum(histogram->GetNbinsY());
}

AzimuthCutStatistics::~AzimuthCutStatistics()
{
}

void AzimuthCutStatistics::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  Q_ASSERT(track);
  double azimuth = track->azimuthAngle() * 180. / M_PI;
  histogram()->Fill(0., azimuth);

  if (!track || !track->fitGood())
    return;
  histogram()->Fill(1., azimuth);

  Enums::EventFlags flags = event->particle()->information()->eventFlags();
  if (!event->flagsSet(Enums::Chi2Good))
    return;
  histogram()->Fill(2., azimuth);

  if (!event->flagsSet(Enums::InsideMagnet))
    return;
  histogram()->Fill(3., azimuth);

  if (!event->flagsSet(Enums::BetaGood))
    return;
  histogram()->Fill(4., azimuth);

  if (event->flagsSet(Enums::Albedo))
    return;
  histogram()->Fill(5., azimuth);
}

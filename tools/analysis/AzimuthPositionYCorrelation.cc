#include "AzimuthPositionYCorrelation.hh"

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

AzimuthPositionYCorrelation::AzimuthPositionYCorrelation(Type type)
  : AnalysisPlot(AnalysisPlot::Tracking)
  , H2DProjectionPlot()
  , m_type(type)
{
  setTitle("Azimuth distribution");

  QString htitle = "Azimuth correlation with y position";

  if (m_type == Positive)
    htitle += " positive";
  if (m_type == Negative)
    htitle += " negative";
  if (m_type == All)
    htitle += " all";
  setTitle(htitle);

  const int nBinsX = 45;
  const double xMin = -180;
  const double xMax = 180;

  const int nBinsY = 40;
  const double yMin = -Constants::tofBarLength / 2.;
  const double yMax = Constants::tofBarLength / 2.;

  TH2D* histogram = new TH2D(qPrintable(htitle), "", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  histogram->Sumw2();
  setAxisTitle("azimuth", "y position / mm", "");
  addHistogram(histogram);

  setDrawOption(COLZ);
  controlWidget()->spinBox()->setMaximum(histogram->GetNbinsY());
}

AzimuthPositionYCorrelation::~AzimuthPositionYCorrelation()
{
}

void AzimuthPositionYCorrelation::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if ( !(flags & ParticleInformation::Chi2Good) || !(flags & ParticleInformation::InsideMagnet) || !(flags & ParticleInformation::BetaGood) || (flags & ParticleInformation::Albedo) )
    return;

  double rigidity = track->rigidity();
  if (m_type == Positive && rigidity < 0) {
    return;
  }
  if (m_type == Negative && rigidity > 0) {
    return;
  }

  double azimuth = track->azimuthAngle() * 180. / M_PI;
  double yPosition = track->y(Constants::upperTofPosition);

  histogram()->Fill(azimuth, yPosition);
}

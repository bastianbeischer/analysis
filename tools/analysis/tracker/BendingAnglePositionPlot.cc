#include "BendingAnglePositionPlot.hh"
#include "BrokenLine.hh"

#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "Track.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TCanvas.h>

#include <cmath>

BendingAnglePositionPlot::BendingAnglePositionPlot(double cut)
  : AnalysisPlot(AnalysisPlot::Tracking)
  , H2DPlot()
  , m_cut(cut)
  , m_normHisto(0)
{
  setTitle(QString("position of tracks with abs(alpha) > %1 rad").arg(cut));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 30, -250, 250, 24, -120, 120);
  setAxisTitle("y / mm", "x / mm", "");
  addHistogram(histogram);
  m_normHisto = new TH2D(qPrintable(title()+"_all"), "", 30, -250, 250, 24, -120, 120);
  setDrawOption(LEGO);
}

BendingAnglePositionPlot::~BendingAnglePositionPlot()
{
  delete m_normHisto;
}

void BendingAnglePositionPlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;

  double alpha = track->bendingAngle();

  if (alpha > m_cut)
    histogram()->Fill(track->y(0), track->x(0));

  m_normHisto->Fill(track->y(0), track->x(0));
}

void BendingAnglePositionPlot::finalize()
{
  histogram()->Divide(m_normHisto);
}

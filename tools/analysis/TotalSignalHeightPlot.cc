#include "TotalSignalHeightPlot.hh"

#include <TH1D.h>

#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Hit.hh"
#include "Cluster.hh"

TotalSignalHeightPlot::TotalSignalHeightPlot() :
  AnalysisPlot(Enums::MiscellaneousTracker),
  H1DPlot()
{
  double maximum = 5e4;
  int bins = 1000;
  setTitle(QString("sum of all signal heights"));
  TH1D* histogram = new TH1D(qPrintable(title()), "", bins, 0, maximum);
  setAxisTitle("signal height / adc counts", "entries");
  addHistogram(histogram);
}

TotalSignalHeightPlot::~TotalSignalHeightPlot()
{
}

void TotalSignalHeightPlot::processEvent(const QVector<Hit*>& hits, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if(!track)
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;

  double sum = 0;

  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tracker) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      sum += cluster->signalHeight();
    }
  }
  histogram()->Fill(sum);
}

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

  addRequiredEventFlags(Enums::TrackGood | Enums::AllTrackerLayers);
}

TotalSignalHeightPlot::~TotalSignalHeightPlot()
{
}

void TotalSignalHeightPlot::processEvent(const AnalyzedEvent* event)
{
  double sum = 0;
  const QVector<Hit*>::const_iterator endIt = event->clusters().end();
  for (QVector<Hit*>::const_iterator it = event->clusters().begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tracker) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      sum += cluster->signalHeight();
    }
  }
  histogram()->Fill(sum);
}

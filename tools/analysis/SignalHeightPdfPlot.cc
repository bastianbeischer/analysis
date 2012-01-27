#include "SignalHeightPdfPlot.hh"

#include "Hit.hh"
#include "Track.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "TimeOfFlight.hh"
#include "TRDReconstruction.hh"
#include "Hypothesis.hh"
#include "SimpleEvent.hh"

#include <TH2.h>

#include <QDebug>

SignalHeightPdfPlot::SignalHeightPdfPlot(Hit::ModuleType type, Enums::Particles particles, const QVector<double>& xBins, const QVector<double>& yBins, int layer)
  : AnalysisPlot(Enums::LikelihoodTopic)
  , H2DPlot()
  , m_type(type)
  , m_particles(particles)
  , m_layer(layer)
{
  QString typeString;
  if (type == Hit::tracker) {
    typeString = "tracker";
  } else if (type == Hit::tof) {
    typeString = "tof";
  } else if (type == Hit::trd) {
    typeString = "trd";
  }
  QString title = QString("signal height pdf %1 %2").arg(typeString).arg(Enums::label(particles));
  if (type == Hit::trd && 0 <= layer && layer < 8)
    title+= " layer " + QString::number(layer);
  TH2D* h = new TH2D(qPrintable(title), "", xBins.count() - 1, xBins.constData(), yBins.count() - 1, yBins.constData());
  addHistogram(h);
  setAxisTitle("|R| / GV", "signal height / a.u.", "");
  setDrawOption(RootPlot::COLZ);
  setTitle(title);
}

SignalHeightPdfPlot::~SignalHeightPdfPlot()
{
}

void SignalHeightPdfPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::AllTrackerLayers | ParticleInformation::InsideMagnet | ParticleInformation::Chi2Good | ParticleInformation::BetaGood))
    return;
  const Hypothesis* h = event->particle()->hypothesis();
  if (h->particle() & m_particles) {
    if (m_type == Hit::tracker) {
      histogram()->Fill(h->absoluteRigidity(), event->particle()->track()->signalHeight());
    } else if (m_type == Hit::tof) {
      histogram()->Fill(h->absoluteRigidity(), event->particle()->timeOfFlight()->timeOverThreshold());
    } else if (m_type == Hit::trd) {
      if (0 <= m_layer) {
        const EnergyDeposition& deposition = event->particle()->trdReconstruction()->energyDepositionForLayer(m_layer);
        histogram()->Fill(h->absoluteRigidity(), deposition.edepOnTrackPerLength);
      } else for (int layer = 0; layer < 8; ++layer){
        const EnergyDeposition& deposition = event->particle()->trdReconstruction()->energyDepositionForLayer(layer);
        histogram()->Fill(h->absoluteRigidity(), deposition.edepOnTrackPerLength);
      }
    }
  }
}

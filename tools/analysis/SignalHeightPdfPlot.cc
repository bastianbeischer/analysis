#include "SignalHeightPdfPlot.hh"

#include "Hit.hh"
#include "Track.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "TimeOfFlight.hh"
#include "TRDReconstruction.hh"
#include "Hypothesis.hh"

#include <TH2.h>

#include <QDebug>

SignalHeightPdfPlot::SignalHeightPdfPlot(Hit::ModuleType type, Enums::Particle particle, const QVector<double>& xBins, const QVector<double>& yBins)
  : AnalysisPlot(Enums::LikelihoodTopic)
  , H2DPlot()
  , m_type(type)
  , m_particle(particle)
{
  QString typeString;
  if (type == Hit::tracker) {
    typeString = "tracker";
  } else if (type == Hit::tof) {
    typeString = "tof";
  } else if (type == Hit::trd) {
    typeString == "trd";
  }
  QString title = QString("signal height pdf %1 %2").arg(typeString).arg(Enums::label(particle));
  TH2D* h = new TH2D(qPrintable(title), "", xBins.count() - 1, xBins.constData(), yBins.count() - 1, yBins.constData());
  addHistogram(h);
  setAxisTitle("|R| / GV", "signal height / a.u.", "");
  setDrawOption(RootPlot::COLZ);
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
  if (m_particle == Enums::NoParticle || h->particle() == m_particle) {
    double signal = 0;
    if (m_type == Hit::tracker) {
      signal = event->particle()->track()->signalHeight();
    } else if (m_type == Hit::tof) {
      signal = event->particle()->timeOfFlight()->timeOverThreshold();
    } else if (m_type == Hit::trd) {
      //TODO: handle layers
      const EnergyDeposition& deposition = event->particle()->trdReconstruction()->energyDepositionForLayer(0);
      signal = deposition.edepOnTrackPerLength;
    }
    histogram()->Fill(h->absoluteRigidity(), signal);
  }
}

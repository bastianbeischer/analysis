#include "ReconstructionMethodCorrelationPlot.hh"
#include "BrokenLine.hh"

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Hypothesis.hh"
#include "SimpleEvent.hh"

#include <TH2.h>

#include <QDebug>

ReconstructionMethodCorrelationPlot::ReconstructionMethodCorrelationPlot(Enums::ReconstructionMethod methodX, Enums::ReconstructionMethod methodY, Enums::Particle particle)
  : AnalysisPlot(Enums::MomentumReconstruction)
  , H2DPlot()
  , m_particle(particle)
  , m_methodX(methodX)
  , m_methodY(methodY)
{
  setTitle("reconstruction method correlation " + Enums::label(methodX) + " " + Enums::label(methodY) + " " + Enums::label(particle));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 240, -12., 12., 240, -12., 12.);
  setAxisTitle("R_{" + Enums::label(methodX) + "} / GV", "R_{" + Enums::label(methodY) + "} / GV" , "");
  addHistogram(histogram);
}

ReconstructionMethodCorrelationPlot::~ReconstructionMethodCorrelationPlot()
{
}

void ReconstructionMethodCorrelationPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(Enums::AllTrackerLayers | Enums::InsideMagnet | Enums::Chi2Good | Enums::BetaGood))
    return;
  const Hypothesis* hypothesisX = event->particle()->hypothesis(m_methodX);
  Q_ASSERT(hypothesisX);
  const Hypothesis* hypothesisY = event->particle()->hypothesis(m_methodY);
  Q_ASSERT(hypothesisY);
  if ((m_particle == Enums::NoParticle) || ((hypothesisX->particle() == m_particle) && (hypothesisY->particle() == m_particle)))
    histogram()->Fill(hypothesisX->rigidity(), hypothesisY->rigidity());
}

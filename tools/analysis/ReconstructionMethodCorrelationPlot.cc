#include "ReconstructionMethodCorrelationPlot.hh"
#include "BrokenLine.hh"

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Hypothesis.hh"

#include <TH2.h>

#include <QDebug>

ReconstructionMethodCorrelationPlot::ReconstructionMethodCorrelationPlot(Enums::ReconstructionMethod methodX, Enums::ReconstructionMethod methodY)
  : AnalysisPlot(Enums::MomentumReconstruction)
  , H2DPlot()
  , m_methodX(methodX)
  , m_methodY(methodY)
{
  setTitle("reconstruction method correlation " + Enums::label(methodX) + " " + Enums::label(methodY));
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
  if (!event->flagsSet(ParticleInformation::AllTrackerLayers | ParticleInformation::Chi2Good | ParticleInformation::BetaGood))
    return;

  const Hypothesis* hypothesisX = event->particle()->hypothesis(m_methodX);
  const Hypothesis* hypothesisY = event->particle()->hypothesis(m_methodY);

  histogram()->Fill(hypothesisX->rigidity(), hypothesisY->rigidity());
}

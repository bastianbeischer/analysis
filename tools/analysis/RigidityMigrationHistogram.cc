#include "RigidityMigrationHistogram.hh"

#include <TH2D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "ParticleProperties.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"
#include "Helpers.hh"
#include "Hypothesis.hh"
#include "ParticleDB.hh"

#include <iostream>
#include <cmath>

RigidityMigrationHistogram::RigidityMigrationHistogram()
  : AnalysisPlot(Enums::MonteCarloTracker)
  , H2DPlot()
{
  QString title = "Rigidity migration";
  setTitle(title);

  QVector<double> axisGenerated = Helpers::rigidityBinning(false);
  int axisSize = 2 * axisGenerated.size();
  for (int i = 0; i < axisSize; i+= 2) {
    double value = axisGenerated.at(i);
    axisGenerated.prepend(-value);
  }

  QVector<double> axisData = Helpers::rigidityBinning(true);
  axisSize = 2 * axisData.size();
  for (int i = 0; i < axisSize; i+= 2) {
    double value = axisData.at(i);
    axisData.prepend(-value);
  }

  TH2D* histogram = new TH2D(qPrintable(title), "", axisData.size()-1, axisData.constData(),  axisGenerated.size()-1, axisGenerated.constData());
  histogram->Sumw2();
  setAxisTitle("reconstructed rigidity", "generated rigidity", "");
  addHistogram(histogram);
  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good | Enums::AllTrackerLayers | Enums::InsideMagnet);
  addRequiredEventFlagsAbsence(Enums::Albedo);
}

RigidityMigrationHistogram::~RigidityMigrationHistogram()
{
}

void RigidityMigrationHistogram::processEvent(const AnalyzedEvent* event)
{
  if (event->simpleEvent()->contentType() != SimpleEvent::MonteCarlo)
    return;
  if (event->simpleEvent()->MCInformation()->primary()->initialMomentum.Z() > 0)
    return;
  if (!event->simpleEvent()->MCInformation()->primary()->isInsideMagnet())
    return;
  int mcPdgId = event->simpleEvent()->MCInformation()->primary()->pdgID;
  const ParticleProperties* mcParticle = ParticleDB::instance()->lookupPdgId(mcPdgId);
  const double charge = mcParticle->charge();
  if (charge == 0)
    return;
  double rigidityGenerated = event->simpleEvent()->MCInformation()->primary()->initialMomentum.Mag() / charge;
  if (rigidityGenerated < histogram()->GetYaxis()->GetBinLowEdge(1) || histogram()->GetYaxis()->GetBinLowEdge(histogram()->GetNbinsY()+1) <= rigidityGenerated)
    return;
  double rigidityData = event->particle()->hypothesis()->rigidity();
  histogram()->Fill(rigidityData, rigidityGenerated);
}

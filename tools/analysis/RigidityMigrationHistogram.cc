#include "RigidityMigrationHistogram.hh"

#include <TH2D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"
#include "Helpers.hh"

#include <iostream>
#include <cmath>

RigidityMigrationHistogram::RigidityMigrationHistogram()
  : AnalysisPlot(Enums::MonteCarloTracker)
  , H2DPlot()
{
  QString title = "Rigidity migration";
  setTitle(title);

  const int nBinsGenerated = EfficiencyCorrectionSettings::numberOfBins(EfficiencyCorrectionSettings::Unfolded);
  const double minGenerated = 0.1;
  const double maxGenerated = 20;
  QVector<double> axisGenerated = Helpers::logBinning(nBinsGenerated, minGenerated, maxGenerated);
  int axisSize = axisGenerated.size()*2;
  for (int i = 0; i < axisSize; i+=2) {
    double value = axisGenerated.at(i);
    axisGenerated.prepend(-value);
  }
  const int nBinsData = EfficiencyCorrectionSettings::numberOfBins(EfficiencyCorrectionSettings::Raw);
  const double minData = 0.1;
  const double maxData = 20;
  QVector<double> axisData = Helpers::logBinning(nBinsData, minData, maxData);
  axisSize = axisData.size()*2;
  for (int i = 0; i < axisSize; i+=2) {
    double value = axisData.at(i);
    axisData.prepend(-value);
  }

  TH2D* histogram = new TH2D(qPrintable(title), "", axisData.size()-1, axisData.constData(),  axisGenerated.size()-1, axisGenerated.constData());
  histogram->Sumw2();
  setAxisTitle("reconstructed rigidity", "generated rigidity", "");
  addHistogram(histogram);
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
  Particle mcParticle(mcPdgId);
  const double charge = mcParticle.charge();
  if (charge == 0)
    return;
  double rigidityGenerated = event->simpleEvent()->MCInformation()->primary()->initialMomentum.Mag() / charge;
  if (rigidityGenerated < histogram()->GetYaxis()->GetBinLowEdge(1) || histogram()->GetYaxis()->GetBinLowEdge(histogram()->GetNbinsY()+1) <= rigidityGenerated)
    return;
  const Track* track = event->particle()->track();
  if (!track || !track->fitGood())
     return;
  ParticleInformation::Flags flags = event->particle()->information()->flags();
  if (!(flags & (ParticleInformation::Chi2Good)))
    return;
  if ( !(flags & ParticleInformation::AllTrackerLayers) || !(flags & ParticleInformation::InsideMagnet) || (flags & ParticleInformation::Albedo) )
    return;
  double rigidityData = track->rigidity();
  histogram()->Fill(rigidityData, rigidityGenerated);
}

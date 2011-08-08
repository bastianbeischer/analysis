#include "RigidityMigrationHistogram.hh"

#include <TH2D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"
#include "Helpers.hh"

#include <iostream>
#include <cmath>

RigidityMigrationHistogram::RigidityMigrationHistogram(Type type)
: AnalysisPlot(AnalysisPlot::MomentumReconstruction)
, H2DPlot()
, m_type(type)
{
  QString title = "Rigidity migration";
  if (m_type == Negative) {
    title += " - negative";
  }
  if (m_type == Positive) {
    title += " - positive";
  }
  if (m_type == All) {
    title += " - all";
  }
  setTitle(title);

  const int nBinsGenerated = 21;
  const double minGenerated = 0.1;
  const double maxGenerated = 20;
  const QVector<double>& axisGenerated = Helpers::logBinning(nBinsGenerated, minGenerated, maxGenerated);

  const int nBinsData = 42;
  const double minData = 0.1;
  const double maxData = 20;
  const QVector<double>& axisData = Helpers::logBinning(nBinsData, minData, maxData);

  TH2D* histogram = new TH2D(qPrintable(title), "", nBinsData, axisData.constData(),  nBinsGenerated, axisGenerated.constData());
  histogram->Sumw2();
  setAxisTitle("reconstructed rigidity", "generated rigidity", "");
  addHistogram(histogram);
}

RigidityMigrationHistogram::~RigidityMigrationHistogram()
{
}

void RigidityMigrationHistogram::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
{
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;

  if (event->MCInformation()->primary()->initialMomentum.Z() > 0)
    return;

  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & (ParticleInformation::Chi2Good)))
    return;
  if ( !(flags & ParticleInformation::AllTrackerLayers) || !(flags & ParticleInformation::InsideMagnet) || (flags & ParticleInformation::Albedo) )
    return;

  int mcPdgId = event->MCInformation()->primary()->pdgID;
  Particle mcParticle(mcPdgId);
  double rigidityGenerated = event->MCInformation()->primary()->initialMomentum.Mag() / mcParticle.charge();

  double rigidityData = track->rigidity();

  if (m_type == Negative && rigidityData >= 0)
    return;
  if (m_type == Positive && rigidityData < 0)
    return;

  histogram()->Fill(qAbs(rigidityData), qAbs(rigidityGenerated));
}

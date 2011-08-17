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

RigidityMigrationHistogram::RigidityMigrationHistogram()
: AnalysisPlot(AnalysisPlot::MomentumReconstruction)
, H2DPlot()
{
  QString title = "Rigidity migration";
  setTitle(title);

  const int nBinsGenerated = 21;
  const double minGenerated = 0.1;
  const double maxGenerated = 20;
  QVector<double> axisGenerated = Helpers::logBinning(nBinsGenerated, minGenerated, maxGenerated);
  int axisSize = axisGenerated.size()*2;
  for (int i = 0; i < axisSize; i+=2) {
    double value = axisGenerated.at(i);
    axisGenerated.prepend(-value);
  }
  const int nBinsData = 42;
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

  m_random = new TRandom3();
  m_random->SetSeed(0);
}

RigidityMigrationHistogram::~RigidityMigrationHistogram()
{
}

void RigidityMigrationHistogram::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;
  if (event->MCInformation()->primary()->initialMomentum.Z() > 0)
    return;
  if (!event->MCInformation()->primary()->isInsideMagnet())
    return;
  int mcPdgId = event->MCInformation()->primary()->pdgID;
  Particle mcParticle(mcPdgId);
  const double charge = mcParticle.charge();
  if (charge == 0)
    return;
  double rigidityGenerated = event->MCInformation()->primary()->initialMomentum.Mag() / charge;
  if (rigidityGenerated < histogram()->GetYaxis()->GetBinLowEdge(1) || histogram()->GetYaxis()->GetBinLowEdge(histogram()->GetNbinsY()+1) <= rigidityGenerated)
    return;
  for (int i = 0; i < 50; ++i) {
    double rigidityData = smearedRigidity(event->MCInformation()->primary(), m_random);
    //TODO take non gaussian tails into account if neccessary
    histogram()->Fill(rigidityData, rigidityGenerated);
  }
}

double RigidityMigrationHistogram::smearedRigidity(const MCSimpleEventParticle* mcParticle, TRandom3* ran, double factor) {
  Particle particle(mcParticle->pdgID);
  const double charge = particle.charge();
  
 	double smearedRigidity = smearedMomentum(mcParticle, ran, factor);
  if (charge != 0) {
    smearedRigidity = smearedRigidity / charge;
  }
	return smearedRigidity;
}

double RigidityMigrationHistogram::smearedMomentum(const MCSimpleEventParticle* mcParticle, TRandom3* ran, double factor) {
  double momentum = mcParticle->initialMomentum.Mag();
  double beta = betaFromMc(mcParticle);
	const double sigmaPoverP = perdaixMomentumResolution(momentum, beta, factor);
	return momentum /  ran->Gaus(1, sigmaPoverP);
}

double RigidityMigrationHistogram::perdaixMomentumResolution(double momentum, double beta, double factor) {
	return sqrt( pow(0.25 / qAbs(beta), 2) + pow(0.08 * momentum, 2)) * factor;
}

double RigidityMigrationHistogram::betaFromMc(const MCSimpleEventParticle* mcParticle) {
	Particle particle(mcParticle->pdgID);
  double m = particle.mass();
  double p = mcParticle->initialMomentum.Mag();
  return 1. / sqrt(1 + pow(m / p, 2));
}

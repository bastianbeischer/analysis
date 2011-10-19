#include "LikelihoodAnalysis.hh"
#include "Particle.hh"
#include "Likelihood.hh"

#include <TGraph.h>
#include <TMultiGraph.h>

LikelihoodAnalysis::LikelihoodAnalysis(Enums::LikelihoodVariables likelihoods)
  : m_likelihoods()
  , m_particle(0)
  , m_mg(new TMultiGraph)
{
  for (Enums::LikelihoodVariableIterator it = Enums::likelihoodVariableBegin(); it != Enums::likelihoodVariableEnd(); ++it)
    if (it.key() & likelihoods)
      m_likelihoods.append(Likelihood::newLikelihood(it.key()));
}

LikelihoodAnalysis::~LikelihoodAnalysis()
{
  qDeleteAll(m_likelihoods);
  delete m_mg;
}

void LikelihoodAnalysis::identify(Particle* particle)
{
  m_particle = particle;
  if (m_mg)
    delete m_mg;
  m_mg = new TMultiGraph;
  double pMin = 0.1;
  double pMax = 10.0;
  double pStep = 0.1;
  int nSteps = (pMax - pMin) / pStep;
  QVector<Enums::Particle> particles = QVector<Enums::Particle>()
    << Enums::Proton << Enums::Electron;
  foreach (Enums::Particle type, particles) {
    TGraph* g = new TGraph(nSteps);
    for (int i = 0; i < nSteps; ++i) {
      g->SetLineColor(Particle(type).color());
      g->SetMarkerSize(0.1);
      double p = pMin + i * pStep;
      g->SetPoint(i, p, test(type, p));
    }
    m_mg->Add(g);
  }

}

double LikelihoodAnalysis::test(Enums::Particle type, double momentum) const
{
  double p = 1;
  for (QVector<Likelihood*>::ConstIterator it = m_likelihoods.begin(); it != m_likelihoods.end(); ++it) {
    p*= 0;
  }
  return p;
}
  
TMultiGraph* LikelihoodAnalysis::graph() const
{
  return m_mg;
}

#include "LikelihoodAnalysis.hh"
#include "Particle.hh"
#include "Likelihood.hh"

#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>

LikelihoodAnalysis::LikelihoodAnalysis(Enums::LikelihoodVariables likelihoods)
  : m_likelihoods()
  , m_particle(0)
  , m_mg(new TMultiGraph)
{
  for (Enums::LikelihoodVariableIterator it = Enums::likelihoodVariableBegin(); it != Enums::likelihoodVariableEnd(); ++it)
    if ((it.key() & likelihoods) == it.key()) {
      qDebug() << Enums::label(it.key());
      m_likelihoods.append(Likelihood::newLikelihood(it.key()));
    }
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
  m_mg->SetTitle(";R / GV;-2 ln L");
  double momentumMin = 0.0;
  double momentumMax = +20.0;
  double momentumStep = 0.1;
  int nSteps = (momentumMax - momentumMin) / momentumStep;
  QVector<Enums::Particle> particles = QVector<Enums::Particle>()
    << Enums::Proton << Enums::AntiProton << Enums::PiPlus << Enums::PiMinus
    << Enums::Electron << Enums::Positron << Enums::Muon << Enums::AntiMuon;

  foreach (Enums::Particle type, particles) {
    TGraph* g = new TGraph(nSteps);
    g->SetName(qPrintable(Enums::label(type)));
    g->SetLineColor(Particle(type).color());
    g->SetMarkerSize(0.1);
    g->SetLineWidth(2);
    for (int i = 0; i < nSteps; ++i) {
      double momentum = momentumMin + i * momentumStep;
      g->SetPoint(i, momentum, test(KineticVariable(type, Enums::Momentum, momentum)));
    }
    m_mg->Add(g);
  }
  qDebug() << particle->track()->rigidity();
}

double LikelihoodAnalysis::test(const KineticVariable& variable) const
{
  double lnL = 0;
  for (QVector<Likelihood*>::ConstIterator it = m_likelihoods.begin(); it != m_likelihoods.end(); ++it) {
    double value = (*it)->eval(m_particle, variable);
    lnL+= -2*log(qMax(1e-9, value));
  }
  return lnL;
}
  
TMultiGraph* LikelihoodAnalysis::graph() const
{
  return m_mg;
}

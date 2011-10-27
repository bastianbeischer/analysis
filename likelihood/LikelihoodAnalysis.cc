#include "LikelihoodAnalysis.hh"
#include "Likelihood.hh"
#include "KineticVariable.hh"
#include "Particle.hh"

#include <Math/IFunction.h>
#include <Math/BrentMinimizer1D.h>
#include <Math/Functor.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>

#include <cfloat>
#include <cmath>

class LogLikelihoodFunction : public ROOT::Math::IGenFunction {
public:
  typedef double (LikelihoodAnalysis::*Function)(const Particle*, const KineticVariable&, bool*) const;

  LogLikelihoodFunction(const LikelihoodAnalysis* analysis, Function function)
    : ROOT::Math::IGenFunction()
    , m_analysis(analysis)
    , m_function(function)
  {
  }

  virtual LogLikelihoodFunction* Clone() const
  {
    return new LogLikelihoodFunction(m_analysis, m_function);
  }

  virtual double DoEval(double x) const
  {
    return (m_analysis->*m_function)(m_particle, KineticVariable(m_particleType, Enums::Momentum, x), m_goodInterpolation);
  }

  void setParameters(const Particle* particle, Enums::Particle particleType, bool* goodInterpolation)
  {
    m_particle = particle;
    m_particleType = particleType;
    m_goodInterpolation = goodInterpolation;
  }

private:
  const LikelihoodAnalysis* m_analysis;
  Function m_function;
  const Particle* m_particle;
  Enums::Particle m_particleType;
  bool* m_goodInterpolation;
};

LikelihoodAnalysis::LikelihoodAnalysis(Enums::LikelihoodVariables likelihoods, Enums::Particles particles)
  : m_likelihoods()
  , m_particles()
  , m_minimizer(new ROOT::Math::BrentMinimizer1D)
  , m_function(new LogLikelihoodFunction(this, &LikelihoodAnalysis::eval))
  , m_minima()
  , m_minimum()
  , m_particle(0)
  , m_mg(0)
{
  m_minimizer->SetFunction(*m_function, 0, 20);

  //m_measuredValueType = Enums::Momentum;
  //m_min = 0;
  //m_max = 20;

  for (Enums::LikelihoodVariableIterator it = Enums::likelihoodVariableBegin(); it != Enums::likelihoodVariableEnd(); ++it)
    if ((it.key() & likelihoods) == it.key())
      m_likelihoods.append(Likelihood::newLikelihood(it.key()));

  for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it)
    if (it.key() != Enums::NoParticle && (it.key() & particles) == it.key())
      m_particles.append(it.key());

  m_minima = QVector<QPointF>(m_particles.count());
}

LikelihoodAnalysis::~LikelihoodAnalysis()
{
  if (m_mg)
    delete m_mg;
  qDeleteAll(m_likelihoods);
}

void LikelihoodAnalysis::identify(Particle* particle)
{
  m_particle = particle;
  bool goodInterpolation = true;
  m_minimum.setX(DBL_MAX);
  m_minimum.setY(DBL_MAX);

  QVector<Enums::Particle>::ConstIterator particleIt = m_particles.begin();
  QVector<Enums::Particle>::ConstIterator particleEnd = m_particles.end();
  QVector<QPointF>::Iterator pointIt = m_minima.begin();
  while (particleIt != particleEnd) {
    m_function->setParameters(particle, *particleIt, &goodInterpolation);
    if (m_minimizer->Minimize(100)) {
      if (m_minimizer->FValMinimum() < m_minimum.y()) {
        m_minimum.setX(m_minimizer->XMinimum());
        m_minimum.setY(m_minimizer->FValMinimum());
      }
      pointIt->setX(m_minimizer->XMinimum());
      pointIt->setY(m_minimizer->FValMinimum());
    } else {
      pointIt->setX(0);
      pointIt->setY(0);
    }
    ++particleIt;
    ++pointIt;
  }
}

double LikelihoodAnalysis::eval(const Particle* particle, const KineticVariable& hypothesis, bool* goodInterpolation) const
{
  double lnL = 0;
  QVector<Likelihood*>::ConstIterator end = m_likelihoods.end();
  for (QVector<Likelihood*>::ConstIterator it = m_likelihoods.begin(); it != end; ++it) {
    double value = (*it)->eval(particle, hypothesis, goodInterpolation);
    if (qIsNull(value))
      return 1e10; //infinity
    lnL+= -2*log(value);
  }
  return lnL;
}

TMultiGraph* LikelihoodAnalysis::graph() const
{
  if (m_mg)
    delete m_mg;
  m_mg = new TMultiGraph;
  m_mg->SetTitle(";R / GV;-2 ln L");
  double momentumMin = 0.0;
  double momentumMax = +20.0;
  double momentumStep = 0.01;
  int nSteps = (momentumMax - momentumMin) / momentumStep;

  foreach (Enums::Particle type, m_particles) {
    TGraph* g = new TGraph(nSteps);
    g->SetName(qPrintable(Enums::label(type)));
    g->SetLineColor(Particle(type).color());
    g->SetMarkerSize(0.1);
    g->SetLineWidth(2);
    for (int i = 0; i < nSteps; ++i) {
      double momentum = momentumMin + i * momentumStep;
      g->SetPoint(i, momentum, eval(m_particle, KineticVariable(type, Enums::Momentum, momentum)));
    }
    m_mg->Add(g, "L");
  }

  TGraph* minimaGraph = new TGraph;
  minimaGraph->SetMarkerStyle(20);
  for (int i = 0; i < m_minima.count(); ++i)
    if (!qFuzzyCompare(m_minima[i].x(), m_minimum.x()) && !qFuzzyCompare(m_minima[i].y(), m_minimum.y()))
      minimaGraph->SetPoint(minimaGraph->GetN(), m_minima[i].x(), m_minima[i].y());
  if (minimaGraph->GetN())
    m_mg->Add(minimaGraph, "P");

  TGraph* minimumGraph = new TGraph(1);
  minimumGraph->SetMarkerStyle(20);
  minimumGraph->SetMarkerColor(kRed);
  minimumGraph->SetPoint(0, m_minimum.x(), m_minimum.y());
  m_mg->Add(minimumGraph, "P");

  return m_mg;
}

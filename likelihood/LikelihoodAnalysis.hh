#ifndef LikelihoodAnalysis_hh
#define LikelihoodAnalysis_hh

#include "Enums.hh"
#include "KineticVariable.hh"

#include <QVector>
#include <QPointF>

class Particle;
class Likelihood;
class TMultiGraph;
class LogLikelihoodFunction;
namespace ROOT {namespace Math {class BrentMinimizer1D;}}

class LikelihoodAnalysis {
public:
  LikelihoodAnalysis(Enums::LikelihoodVariables, Enums::Particles);
  ~LikelihoodAnalysis();

  void identify(Particle*);
  TMultiGraph* graph() const;
private:
  double eval(const Particle*, const KineticVariable& hypothesis, bool* goodInterpolation = 0) const;
  double rootFunctorPointer(const double* momentum);

  QVector<Likelihood*> m_likelihoods;
  QVector<Enums::Particle> m_particles;
  ROOT::Math::BrentMinimizer1D* m_minimizer;
  LogLikelihoodFunction* m_function;
  QVector<QPointF> m_minima;
  QPointF m_minimum;
  Particle* m_particle;
  mutable TMultiGraph* m_mg;
};

#endif

#ifndef LikelihoodAnalysis_hh
#define LikelihoodAnalysis_hh

#include "Enums.hh"
#include "KineticVariable.hh"

#include <QVector>

class Particle;
class Likelihood;
class TMultiGraph;

class LikelihoodAnalysis
{
public:
  LikelihoodAnalysis(Enums::LikelihoodVariables);
  ~LikelihoodAnalysis();

  void identify(Particle*);
  TMultiGraph* graph() const;
private:
  double test(const KineticVariable&) const;
  QVector<Likelihood*> m_likelihoods;
  Particle* m_particle;
  TMultiGraph* m_mg;
};

#endif

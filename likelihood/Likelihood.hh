#ifndef Likelihood_hh
#define Likelihood_hh

#include "Enums.hh"
#include "Hypothesis.hh"
#include "KineticVariable.hh"

#include <QMap>
#include <QVector>
#include <QDebug>

class AnalyzedEvent;
class LikelihoodPDF;

class Likelihood {
public:
  Likelihood(Enums::Particles);
  ~Likelihood();
  Enums::KineticVariable measuredValueType() const;
  Enums::LikelihoodVariable likelihoodVariableType() const;
  static Likelihood* newLikelihood(Enums::LikelihoodVariable, Enums::Particles);
  virtual LikelihoodPDF* pdf(const KineticVariable&) const;
  virtual double min() const;
  virtual double max() const;
  virtual int numberOfParameters() const;

  virtual double eval(const AnalyzedEvent*, const Hypothesis& hypothesis, bool* goodInterpolation = 0) const = 0;
  virtual double eval(double measuredValue, const Hypothesis& hypothesis, bool* goodInterpolation = 0) const = 0;
protected:
  typedef QVector<double> ParameterVector;
  typedef QMap<double, ParameterVector> MomentumMap;
  typedef QMap<Enums::Particle, MomentumMap> ParticleMap;

  virtual MomentumMap::ConstIterator end(Enums::Particle) const;
  virtual MomentumMap::ConstIterator lowerNode(const Hypothesis&) const;
  virtual MomentumMap::ConstIterator upperNode(const Hypothesis&) const;
  virtual ParameterVector defaultParameters() const;
  virtual void loadNodes();
  virtual ParameterVector linearInterpolation(const Hypothesis&, bool* goodInterpolation = 0) const;

  Enums::Particles m_particles;
  Enums::KineticVariable m_measuredValueType;
  Enums::LikelihoodVariable m_likelihoodVariableType;
  double m_min;
  double m_max;
  int m_numberOfParameters;
private:
  ParticleMap m_nodes;
};

#endif

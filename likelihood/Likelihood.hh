#ifndef Likelihood_hh
#define Likelihood_hh

#include "Enums.hh"
#include "Hypothesis.hh"
#include "KineticVariable.hh"

#include <QMap>
#include <QVector>
#include <QString>
#include <QDebug>

class AnalyzedEvent;
class LikelihoodPDF;

class Likelihood {
public:
  typedef QVector<double> ParameterVector;

  Likelihood(Enums::Particles);
  ~Likelihood();

  const QString& title() const;
  Enums::KineticVariable measuredValueType() const;
  Enums::LikelihoodVariable likelihoodVariableType() const;
  static Likelihood* newLikelihood(Enums::LikelihoodVariable, Enums::Particles);
  virtual LikelihoodPDF* pdf(const KineticVariable&) const;
  virtual double min() const;
  virtual double max() const;
  virtual int numberOfParameters() const;
  Enums::Particles particles() const;

  virtual double eval(const AnalyzedEvent*, const Hypothesis& hypothesis, bool* goodInterpolation = 0) const = 0;
  virtual double eval(double measuredValue, const Hypothesis& hypothesis, bool* goodInterpolation = 0) const = 0;
  ParameterVector interpolation(const Hypothesis&, bool* goodInterpolation = 0) const;
  double normalizationInterpolation(const Hypothesis&, bool* goodInterpolation = 0) const;
protected:
  typedef QMap<double, double> NormalizationMap;
  typedef QMap<Enums::Particle, NormalizationMap> NormalizationParticleMap;
  typedef QMap<double, ParameterVector> AbsoluteRigidityMap;
  typedef QMap<Enums::Particle, AbsoluteRigidityMap> ParticleMap;

  typedef QVector<Likelihood::ParameterVector> Parametrization;
  typedef QMap<Enums::Particle, Parametrization> ParametrizationMap;
  typedef QVector<double> ParametrizationNormalization;
  typedef QMap<Enums::Particle, ParametrizationNormalization> ParametrizationNormalizationMap;

  virtual AbsoluteRigidityMap::ConstIterator end(Enums::Particle) const;
  virtual AbsoluteRigidityMap::ConstIterator lowerNode(const Hypothesis&) const;
  virtual AbsoluteRigidityMap::ConstIterator upperNode(const Hypothesis&) const;
  virtual ParameterVector defaultParameters() const;

  virtual NormalizationMap::ConstIterator normalizationEnd(Enums::Particle) const;
  virtual NormalizationMap::ConstIterator normalizationLowerNode(const Hypothesis&) const;
  virtual NormalizationMap::ConstIterator normalizationUpperNode(const Hypothesis&) const;

  ParameterVector calculateInterpolation(const Hypothesis&, bool* goodInterpolation = 0) const;
  double calculateNormalizationInterpolation(const Hypothesis&, bool* goodInterpolation = 0) const;

  virtual void loadNodes();
  void setupParametrizations();
  int valueToIndex(double absoluteRigidity) const;

  QString m_title;
  Enums::Particles m_particles;
  Enums::KineticVariable m_measuredValueType;
  Enums::LikelihoodVariable m_likelihoodVariableType;
  double m_min;
  double m_max;
  int m_numberOfParameters;
  ParticleMap m_nodes;
  NormalizationParticleMap m_normalization;
  
  static const double s_parametrizationMin;
  static const double s_parametrizationMax;
  static const double s_parametrizationNumberOfPoints;
  static const double s_parametrizationStep;
  ParametrizationMap m_parametrizations;
  ParametrizationNormalizationMap m_parametrizationNormalization;
};

#endif

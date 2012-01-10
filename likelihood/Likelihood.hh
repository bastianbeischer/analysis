#ifndef Likelihood_hh
#define Likelihood_hh

#include "Enums.hh"
#include "PDFParameters.hh"
#include "PDFParametersVector.hh"
#include "Hypothesis.hh"
#include "KineticVariable.hh"

#include <QMap>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QPair>
#include <QVariant>

class AnalyzedEvent;
class LikelihoodPDF;

class Likelihood {
public:
  static Likelihood* newLikelihood(Enums::LikelihoodVariable, Enums::Particles);
  Likelihood(Enums::Particles);
  ~Likelihood();

  const QString& title() const;
  Enums::LikelihoodVariable likelihoodVariableType() const;
  Enums::Particles particles() const;
  Enums::KineticVariable measuredValueType() const;
  int numberOfParameters() const;
  double measuredValueMin() const;
  double measuredValueMax() const;
  void saveParameters(const Hypothesis&, const PDFParameters&);
  const PDFParameters& defaultParameters() const;
  LikelihoodPDF* pdf(const KineticVariable&) const;
  virtual double eval(const AnalyzedEvent*, const Hypothesis&, bool* goodInterpolation = 0) const = 0;
  virtual double eval(double measuredValue, const Hypothesis&, bool* goodInterpolation = 0) const = 0;
  virtual double eval(double measuredValue, const Hypothesis&, const PDFParameters&) const = 0;
  virtual const PDFParameters& interpolation(const Hypothesis&, bool* goodInterpolation = 0) const;
protected:
  typedef QMap<Enums::Particle, PDFParametersVector> PDFParametersVectorMap;
  typedef QPair<Enums::Particle, const PDFParametersVector*> Buffer;

  virtual double interpolation(double rigidity, const QVector<double>& rigidities, const QVector<double>& values);
  virtual void loadParameters();

  QString m_title;
  Enums::LikelihoodVariable m_likelihoodVariableType;
  Enums::Particles m_particles;
  Enums::KineticVariable m_measuredValueType;
  int m_numberOfParameters;
  double m_measuredValueMin;
  double m_measuredValueMax;
  const double m_parametersVectorsStep;
  PDFParametersVectorMap m_parametersVectors;
  PDFParameters m_defaultParameters;
  mutable Buffer m_buffer;
};

#endif

#include "Likelihood.hh"
#include "Helpers.hh"
#include "LikelihoodPDF.hh"
#include "TimeOfFlightLikelihood.hh"
#include "TrackerMomentumLikelihood.hh"
#include "CherenkovLikelihood.hh"
#include "SignalHeightTrdLikelihood.hh"
#include "TimeOverThresholdLikelihood.hh"

#include <QSettings>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QVariant>

Likelihood* Likelihood::newLikelihood(Enums::LikelihoodVariable type, Enums::Particles particles)
{
  switch (type) {
    case Enums::UndefinedLikelihood: return 0;
    case Enums::SignalHeightTrackerLikelihood: return 0; //TODO
    case Enums::SignalHeightTrdLikelihood: return new SignalHeightTrdLikelihood(particles);
    case Enums::TimeOverThresholdLikelihood: return new TimeOverThresholdLikelihood(particles);
    case Enums::TimeOfFlightLikelihood: return new TimeOfFlightLikelihood(particles);
    case Enums::TrackerMomentumLikelihood: return new TrackerMomentumLikelihood(particles);
    case Enums::CherenkovLikelihood: return new CherenkovLikelihood(particles);
  }
  return 0;
}

Likelihood::Likelihood(Enums::Particles particles)
  : m_title("no title")
  , m_likelihoodVariableType()
  , m_particles(particles)
  , m_measuredValueType(Enums::UndefinedKineticVariable)
  , m_numberOfParameters(0)
  , m_measuredValueMin(0)
  , m_measuredValueMax(0)
  , m_parametersVectorsStep(0.01) //GV
  , m_parametersVectors()
  , m_buffer(Buffer(Enums::NoParticle, 0))
{
}

Likelihood::~Likelihood()
{
}

const QString& Likelihood::title() const
{
  return m_title;
}

Enums::LikelihoodVariable Likelihood::likelihoodVariableType() const
{
  return m_likelihoodVariableType;
}

Enums::Particles Likelihood::particles() const
{
  return m_particles;
}

Enums::KineticVariable Likelihood::measuredValueType() const
{
  return m_measuredValueType;
}

int Likelihood::numberOfParameters() const
{
  return m_numberOfParameters;
}

double Likelihood::measuredValueMin() const
{
  return m_measuredValueMin;
}

double Likelihood::measuredValueMax() const
{
  return m_measuredValueMax;
}

const PDFParameters& Likelihood::defaultParameters() const
{
  return m_defaultParameters;
}

LikelihoodPDF* Likelihood::pdf(const KineticVariable& variable) const
{
  if (variable.particle() & m_particles) {
    LikelihoodPDF* pdf = new LikelihoodPDF(this, variable);
    pdf->setParameters(interpolation(Hypothesis(variable.particle(), variable.curvature())));
    return pdf;
  }
  return 0;
}

const PDFParameters& Likelihood::interpolation(const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  double absoluteRigidity = hypothesis.absoluteRigidity();
  Enums::Particle particle = hypothesis.particle();
  if (m_buffer.first != particle) {
    PDFParametersVectorMap::ConstIterator pIt = m_parametersVectors.find(particle);
    if (pIt == m_parametersVectors.constEnd())
      return m_defaultParameters;
    Q_ASSERT(pIt != m_parametersVectors.constEnd());
    m_buffer.first = particle;
    m_buffer.second = &pIt.value();
  }
  double min = m_buffer.second->min();
  double max = m_buffer.second->max();
  if (goodInterpolation)
    *goodInterpolation = (min <= absoluteRigidity) && (absoluteRigidity <= max);
  if (absoluteRigidity < min)
    return m_buffer.second->first();
  if (absoluteRigidity > max)
    return m_buffer.second->last();
  int index = (absoluteRigidity - min) / m_parametersVectorsStep;//buffer.second->count();
  return (m_buffer.second->constData())[index];
}

double Likelihood::interpolation(double rigidity, const QVector<double>& rigidities, const QVector<double>& values)
{
  Q_ASSERT(rigidities.count() == values.count());
  Q_ASSERT(rigidities.first() <= rigidity && rigidity <= rigidities.last());
  if (qFuzzyCompare(rigidity, rigidities.first()))
    return values.first();
  if (qFuzzyCompare(rigidity, rigidities.last()))
    return values.last();
  int i = 1;
  int n = rigidities.count();
  for (; i < n; ++i) {
    if (rigidity < rigidities[i])
      break;
  }
  return values[i-1] + (rigidity-rigidities[i-1]) * (values[i]-values[i-1]) / (rigidities[i]-rigidities[i-1]);
}

void Likelihood::loadParameters()
{
  QString fileName = Helpers::analysisPath() + "/conf/Likelihood.conf";
  Q_ASSERT(QFile::exists(fileName));
  QSettings settings(fileName, QSettings::IniFormat);
  m_parametersVectors.clear();
  settings.beginGroup(Enums::label(m_likelihoodVariableType));
  Enums::Particles particlesInFile = Enums::particles(settings.value("particles").toString());
  Enums::ParticleIterator particleEnd = Enums::particleEnd();

  for (Enums::ParticleIterator particleIt = Enums::particleBegin(); particleIt != particleEnd; ++particleIt) {
    if (particleIt.key() == Enums::NoParticle || !(particleIt.key() & particlesInFile))
      continue;
    settings.beginGroup(particleIt.value());
    const QVector<double>& rigidities = Helpers::variantToDoubleVector(settings.value("absoluteRigidities"));
    Q_ASSERT(Helpers::sorted(rigidities));
    if (rigidities.size()) {
      double min = rigidities.first();
      double max = rigidities.last();
      int numberOfElements = (max - min) / m_parametersVectorsStep + 1;
      PDFParametersVector parametersVector(numberOfElements, m_numberOfParameters);
      parametersVector.setRange(min, max);
      QVector<double> normalizationRigidities = Helpers::variantToDoubleVector(settings.value("normalizationRigidities"));
      bool normalization = normalizationRigidities.count();
      Q_ASSERT(Helpers::sorted(normalizationRigidities));
      Q_ASSERT(!normalization || qFuzzyCompare(normalizationRigidities.first(), min));
      Q_ASSERT(!normalization || qFuzzyCompare(normalizationRigidities.last(), max));
      QVector<double> normalizationFactors = Helpers::variantToDoubleVector(settings.value("normalizationFactors"));
      Q_ASSERT(normalizationRigidities.count() == normalizationFactors.count());
      for (int i = 0; i < numberOfElements; ++i) {
        double rigidity = min + i * m_parametersVectorsStep;
        double normalizationFactor = 1;
        if (normalization)
          normalizationFactor = interpolation(rigidity, normalizationRigidities, normalizationFactors);
        parametersVector[i].setNormalizationFactor(normalizationFactor);
        for (int parameter = 0; parameter < m_numberOfParameters; ++parameter) {
          QString key = QString::number(parameter);
          const QVector<double>& parameters = Helpers::variantToDoubleVector(settings.value(QString::number(parameter)));
          parametersVector[i][parameter] = interpolation(rigidity, rigidities, parameters);
        }
      }
      m_parametersVectors.insert(particleIt.key(), parametersVector);
    }
    settings.endGroup();
  }
  settings.endGroup();
}

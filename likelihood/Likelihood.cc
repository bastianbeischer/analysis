#include "Likelihood.hh"
#include "Helpers.hh"
#include "LikelihoodPDF.hh"
#include "TimeOfFlightLikelihood.hh"
#include "TrackerMomentumLikelihood.hh"
#include "CherenkovLikelihood.hh"
#include "SignalHeightTrdLikelihood.hh"

#include <QSettings>
#include <QStringList>
#include <QFile>
#include <QDebug>

const double Likelihood::s_parametrizationMin = 0;
const double Likelihood::s_parametrizationMax = 100;
const double Likelihood::s_parametrizationNumberOfPoints = 2002;
const double Likelihood::s_parametrizationStep = (s_parametrizationMax - s_parametrizationMin) / (s_parametrizationNumberOfPoints - 1);

Likelihood::Likelihood(Enums::Particles particles)
  : m_title("no title")
  , m_particles(particles)
  , m_measuredValueType(Enums::UndefinedKineticVariable)
  , m_likelihoodVariableType()
  , m_min(0)
  , m_max(0)
  , m_numberOfParameters(0)
  , m_nodes()
  , m_normalization()
  , m_parametrizations()
  , m_parametrizationBuffer(0)
  , m_parametrizationParticleBuffer(Enums::NoParticle)
  , m_parametrizationNormalizations()
  , m_parametrizationNormalizationBuffer(0)
  , m_parametrizationNormalizationParticleBuffer(Enums::NoParticle)
{
}

Likelihood::~Likelihood()
{
  qDeleteAll(m_parametrizations);
  qDeleteAll(m_parametrizationNormalizations);
}

const QString& Likelihood::title() const
{
  return m_title;
}

Enums::KineticVariable Likelihood::measuredValueType() const
{
  return m_measuredValueType;
}

Enums::LikelihoodVariable Likelihood::likelihoodVariableType() const
{
  return m_likelihoodVariableType;
}

Likelihood::AbsoluteRigidityMap::ConstIterator Likelihood::end(Enums::Particle type) const
{
  return m_nodes[type].end();
}

Likelihood::AbsoluteRigidityMap::ConstIterator Likelihood::lowerNode(const Hypothesis& hypothesis) const
{
  ParticleMap::ConstIterator particleIt = m_nodes.constFind(hypothesis.particle());
  Q_ASSERT(particleIt != m_nodes.constEnd());
  if (!particleIt.value().count())
    qDebug() << hypothesis;
  Q_ASSERT_X(particleIt.value().count(), "Likelihood::lowerNode()", "No momenta found for requested particle species.");
  AbsoluteRigidityMap::ConstIterator momentumIt = particleIt.value().lowerBound(hypothesis.absoluteRigidity());
  if (qFuzzyCompare(momentumIt.key(), hypothesis.absoluteRigidity()))
    return momentumIt;
  return --momentumIt;
}

Likelihood::AbsoluteRigidityMap::ConstIterator Likelihood::upperNode(const Hypothesis& hypothesis) const
{
  return ++lowerNode(hypothesis);
}

int Likelihood::valueToIndex(double absoluteRigidity) const
{
  Q_ASSERT((s_parametrizationMin <= absoluteRigidity) && (absoluteRigidity <= s_parametrizationMax));
  return (absoluteRigidity - s_parametrizationMin) / s_parametrizationStep;
}

Likelihood::ParameterVector Likelihood::interpolation(const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  double absoluteRigidity = hypothesis.absoluteRigidity();
  if (goodInterpolation)
    *goodInterpolation = (s_parametrizationMin <= absoluteRigidity) && (absoluteRigidity <= s_parametrizationMax);
  Enums::Particle particle = hypothesis.particle();
  if (m_parametrizationParticleBuffer != particle) {
    ParametrizationMap::ConstIterator it = m_parametrizations.find(particle);
    Q_ASSERT(it != m_parametrizations.constEnd());
    m_parametrizationBuffer = *it;
    m_parametrizationParticleBuffer = particle;
  }
  Q_ASSERT(m_parametrizationBuffer);
  if (absoluteRigidity < s_parametrizationMin)
    return m_parametrizationBuffer->first();
  if (absoluteRigidity > s_parametrizationMax)
    return  m_parametrizationBuffer->last();
  return (m_parametrizationBuffer->constData())[valueToIndex(absoluteRigidity)];
  return m_parametrizationBuffer->at(valueToIndex(absoluteRigidity));
}

Likelihood::ParameterVector Likelihood::calculateInterpolation(const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  // linear interpolation method. Other methods should be implemented in the
  // according class that inherits Likelihood.
  AbsoluteRigidityMap::ConstIterator endIt = end(hypothesis.particle());
  AbsoluteRigidityMap::ConstIterator l = lowerNode(hypothesis);
  AbsoluteRigidityMap::ConstIterator u = l;
  ++u;
  if (goodInterpolation)
    *goodInterpolation = (l != endIt) && (u != endIt);
  Q_ASSERT(!(l == endIt && u == endIt));
  if (l == endIt)
    return u.value();
  if (u == endIt)
    return l.value();
  if (qFuzzyCompare(l.key(), hypothesis.absoluteRigidity()))
    return l.value();
  double k = (hypothesis.absoluteRigidity() - l.key()) / (u.key() - l.key());
  ParameterVector vector(numberOfParameters());
  for (int i = 0; i < numberOfParameters(); ++i)
    vector[i] = l.value()[i] + k * (u.value()[i] - l.value()[i]);
  return vector;
}

Likelihood::NormalizationMap::ConstIterator Likelihood::normalizationEnd(Enums::Particle p) const
{
  return m_normalization[p].end();
}

Likelihood::NormalizationMap::ConstIterator Likelihood::normalizationLowerNode(const Hypothesis& h) const
{
  NormalizationParticleMap::ConstIterator particleIt = m_normalization.constFind(h.particle());
  Q_ASSERT(particleIt != m_normalization.constEnd());
  if (!particleIt.value().count())
    qDebug() << h;
  Q_ASSERT_X(particleIt.value().count(), "Likelihood::normalizationLowerNode()", "No normalization found for requested particle species.");
  NormalizationMap::ConstIterator momentumIt = particleIt.value().lowerBound(h.absoluteRigidity());
  if (qFuzzyCompare(momentumIt.key(), h.absoluteRigidity()))
    return momentumIt;
  return --momentumIt;
}

Likelihood::NormalizationMap::ConstIterator Likelihood::normalizationUpperNode(const Hypothesis& h) const
{
  return ++normalizationLowerNode(h);
}

double Likelihood::normalizationInterpolation(const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  double absoluteRigidity = hypothesis.absoluteRigidity();
  if (goodInterpolation)
    *goodInterpolation = (s_parametrizationMin <= absoluteRigidity) && (absoluteRigidity <= s_parametrizationMax);
  Enums::Particle particle = hypothesis.particle();
  if (m_parametrizationNormalizationParticleBuffer != particle) {
    ParametrizationNormalizationMap::ConstIterator it = m_parametrizationNormalizations.find(particle);
    Q_ASSERT(it != m_parametrizationNormalizations.constEnd());
    m_parametrizationNormalizationBuffer = *it;
    m_parametrizationNormalizationParticleBuffer = particle;
  }
  Q_ASSERT(m_parametrizationNormalizationBuffer);
  if (absoluteRigidity < s_parametrizationMin)
    return m_parametrizationNormalizationBuffer->first();
  if (absoluteRigidity > s_parametrizationMax)
    return m_parametrizationNormalizationBuffer->last();
  return m_parametrizationNormalizationBuffer->at(valueToIndex(absoluteRigidity));
}

double Likelihood::calculateNormalizationInterpolation(const Hypothesis& h, bool* goodInterpolation) const
{
  NormalizationMap::ConstIterator endIt = normalizationEnd(h.particle());
  NormalizationMap::ConstIterator l = normalizationLowerNode(h);
  NormalizationMap::ConstIterator u = l;
  ++u;
  if (goodInterpolation)
    *goodInterpolation = (l != endIt) && (u != endIt);
  if (l == endIt)
    return u.value();
  if (u == endIt)
    return l.value();
  if (qFuzzyCompare(l.key(), h.absoluteRigidity()))
    return l.value();
  double k = (h.absoluteRigidity() - l.key()) / (u.key() - l.key());
  return l.value() + k * (u.value() - l.value());
}

void Likelihood::loadNodes()
{
  QString fileName = Helpers::analysisPath() + "/conf/Likelihood.conf";
  Q_ASSERT(QFile::exists(fileName));
  QSettings settings(fileName, QSettings::IniFormat);
  m_nodes.clear();
  settings.beginGroup(Enums::label(m_likelihoodVariableType));
  Enums::Particles particlesInFile = Enums::particles(settings.value("particles").toString());
  Enums::ParticleIterator particleEnd = Enums::particleEnd();
  for (Enums::ParticleIterator particleIt = Enums::particleBegin(); particleIt != particleEnd; ++particleIt) {
    if (particleIt.key() == Enums::NoParticle || !(particleIt.key() & particlesInFile))
      continue;
    ParticleMap::Iterator particleNodeIterator = m_nodes.find(particleIt.key());
    if (particleNodeIterator == m_nodes.end())
      particleNodeIterator = m_nodes.insert(particleIt.key(), AbsoluteRigidityMap());
    settings.beginGroup(particleIt.value());

    QList<QVariant> normalizationRrigiditiesVariantList = settings.value("normalizationRigidities").toList();
    QList<QVariant> normalizationFactorsVariantList = settings.value("normalizationFactors").toList();
    Q_ASSERT(normalizationRrigiditiesVariantList.size() == normalizationFactorsVariantList.size());
    NormalizationMap normalizationMap;
    for (int i = 0; i < normalizationFactorsVariantList.size(); ++i)
      normalizationMap.insert(normalizationRrigiditiesVariantList[i].toDouble(), normalizationFactorsVariantList[i].toDouble());
    m_normalization.insert(particleIt.key(), normalizationMap);

    QList<QVariant> rigiditiesVariantList = settings.value("absoluteRigidities").toList();
    foreach(QVariant rigidityVariant, rigiditiesVariantList) {
      double rigidity = rigidityVariant.toDouble();
      QString key = QString("%1GV").arg(rigidityVariant.toString());
      QList<QVariant> variantList = settings.value(key).toList();
      Q_ASSERT(variantList.size() == numberOfParameters());
      AbsoluteRigidityMap::Iterator rigidityNodeIterator = particleNodeIterator.value().find(rigidity);
      if (rigidityNodeIterator == particleNodeIterator.value().end())
        rigidityNodeIterator = particleNodeIterator.value().insert(rigidity, ParameterVector(variantList.size()));
      for (int i = 0; i < variantList.size(); ++i)
        rigidityNodeIterator.value()[i] = variantList[i].toDouble();
    }
    settings.endGroup();
  }
  settings.endGroup();

  setupParametrizations();
}

void Likelihood::setupParametrizations()
{
  Enums::ParticleIterator end = Enums::particleEnd();
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != end; ++it) {
    if (it.key() == Enums::NoParticle || (it.key() & m_particles) != it.key())
      continue;
    Parametrization* parametrization = new Parametrization(s_parametrizationNumberOfPoints, defaultParameters());
    ParametrizationNormalization* normalization = new ParametrizationNormalization(s_parametrizationNumberOfPoints, 1);
    for (int i = 0; i < s_parametrizationNumberOfPoints; ++i) {
      double absoluteRigidity = s_parametrizationMin + i * s_parametrizationStep;
      Hypothesis h(it.key(), 1. / absoluteRigidity);
      (*parametrization)[i] = calculateInterpolation(h);
      (*normalization)[i] = calculateNormalizationInterpolation(h);
    }
    m_parametrizations.insert(it.key(), parametrization);
    m_parametrizationNormalizations.insert(it.key(), normalization);
  }
}

LikelihoodPDF* Likelihood::pdf(const KineticVariable& variable) const
{
  if (variable.particle() & m_particles)
    return new LikelihoodPDF(this, variable);
  return 0;
}

double Likelihood::min() const
{
  return m_min;
}

double Likelihood::max() const
{
  return m_max;
}

int Likelihood::numberOfParameters() const
{
  return m_numberOfParameters;
}

Enums::Particles Likelihood::particles() const
{
  return m_particles;
}

Likelihood* Likelihood::newLikelihood(Enums::LikelihoodVariable type, Enums::Particles particles)
{
  switch (type) {
    case Enums::UndefinedLikelihood: return 0;
    case Enums::SignalHeightTrackerLikelihood: return 0;
    case Enums::SignalHeightTrdLikelihood: return new SignalHeightTrdLikelihood(particles);
    case Enums::TimeOverThresholdLikelihood: return 0;
    case Enums::TimeOfFlightLikelihood: return new TimeOfFlightLikelihood(particles);
    case Enums::TrackerMomentumLikelihood: return new TrackerMomentumLikelihood(particles);
    case Enums::CherenkovLikelihood: return new CherenkovLikelihood(particles);
  }
  return 0;
}

Likelihood::ParameterVector Likelihood::defaultParameters() const
{
  return ParameterVector(numberOfParameters());
}

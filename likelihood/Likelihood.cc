#include "Likelihood.hh"
#include "Helpers.hh"
#include "LikelihoodPDF.hh"
#include "TimeOfFlightLikelihood.hh"
#include "TrackerMomentumLikelihood.hh"

#include <QSettings>
#include <QStringList>
#include <QFile>
#include <QDebug>

Likelihood::Likelihood()
  : m_measuredValueType(Enums::UndefinedKineticVariable)
  , m_likelihoodVariableType()
  , m_min(0)
  , m_max(0)
  , m_numberOfParameters(0)
  , m_nodes()
{
}

Likelihood::~Likelihood()
{
}

Enums::KineticVariable Likelihood::measuredValueType() const
{
  return m_measuredValueType;
}

Enums::LikelihoodVariable Likelihood::likelihoodVariableType() const
{
  return m_likelihoodVariableType;
}

Likelihood::MomentumMap::ConstIterator Likelihood::end(Enums::Particle type) const
{
  return m_nodes[type].end();
}

Likelihood::MomentumMap::ConstIterator Likelihood::lowerNode(const KineticVariable& variable) const
{
  ParticleMap::ConstIterator particleIt = m_nodes.find(variable.particle());
  MomentumMap::ConstIterator momentumIt = particleIt.value().lowerBound(variable.rigidity());
  if (qFuzzyCompare(momentumIt.key(), variable.rigidity()))
    return momentumIt;
  return --momentumIt;
}

Likelihood::MomentumMap::ConstIterator Likelihood::upperNode(const KineticVariable& variable) const
{
  return ++lowerNode(variable);
}

Likelihood::ParameterVector Likelihood::linearInterpolation(const KineticVariable& variable, bool* goodInterpolation) const
{
  MomentumMap::ConstIterator endIt = end(variable.particle());
  MomentumMap::ConstIterator l = lowerNode(variable);
  if (qFuzzyCompare(l.key(), variable.rigidity()))
    return l.value();
  MomentumMap::ConstIterator u = l;
  ++u;
  if (l == endIt || u == endIt) {
    if (goodInterpolation) {
      *goodInterpolation = false;
    } else {
      qWarning()
        << "No values for an interpolation of" << Enums::label(m_likelihoodVariableType)
        << "for" << Enums::label(variable.particle()) << "at R =" << variable.rigidity() << "GV.";
    }
    return defaultParameters();
  }
  if (goodInterpolation)
    *goodInterpolation = true;
  double k = (variable.rigidity() - l.key()) / (u.key() - l.key());
  ParameterVector vector(numberOfParameters());
  for (int i = 0; i < numberOfParameters(); ++i)
    vector[i] = l.value()[i] + k * (u.value()[i] - l.value()[i]);
  return vector;
}

void Likelihood::loadNodes()
{
  QString fileName = Helpers::analysisPath() + "/conf/Likelihood.conf";
  Q_ASSERT(QFile::exists(fileName));
  QSettings settings(fileName, QSettings::IniFormat);
  settings.beginGroup(Enums::label(m_likelihoodVariableType));
  
  foreach (QString particleKey, settings.childGroups()) {
    settings.beginGroup(particleKey);
    Enums::Particle particle = Enums::particle(particleKey);
    ParticleMap::Iterator particleIt = m_nodes.find(particle);
    if (particleIt == m_nodes.end())
      particleIt = m_nodes.insert(particle, MomentumMap());
    foreach (QString momentumKey, settings.childKeys()) {
      double p = momentumKey.toDouble();
      QVariant variant = settings.value(momentumKey);
      variant.convert(QVariant::StringList);
      QList<QVariant> variantList = variant.toList();
      Q_ASSERT(variantList.size() == numberOfParameters());
      MomentumMap::Iterator momentumIt = particleIt.value().find(p);
      if (momentumIt == particleIt.value().end())
        momentumIt = particleIt.value().insert(p, ParameterVector(variantList.size()));
      for (int i = 0; i < variantList.size(); ++i)
        momentumIt.value()[i] = variantList[i].toDouble();
    }
    settings.endGroup();
  }
  settings.endGroup();
}

LikelihoodPDF* Likelihood::pdf(const KineticVariable& variable) const
{
  return new LikelihoodPDF(this, variable);
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

Likelihood* Likelihood::newLikelihood(Enums::LikelihoodVariable type)
{
  switch (type) {
    case Enums::UndefinedLikelihood: return 0;
    case Enums::SignalHeightTrackerLikelihood: return 0;
    case Enums::SignalHeightTRDLikelihood: return 0;
    case Enums::TimeOverThresholdLikelihood: return 0;
    case Enums::TimeOfFlightLikelihood: return new TimeOfFlightLikelihood();
    case Enums::TrackerMomentumLikelihood: return new TrackerMomentumLikelihood();
  }
  return 0;
}

Likelihood::ParameterVector Likelihood::defaultParameters() const
{
  return ParameterVector(numberOfParameters());
}
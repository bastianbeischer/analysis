#include "Likelihood.hh"
#include "Helpers.hh"
#include "LikelihoodPDF.hh"
#include "LikelihoodRatio.hh"
#include "TimeOfFlightLikelihood.hh"
#include "TrackerMomentumLikelihood.hh"

#include <QSettings>
#include <QStringList>
#include <QFile>
#include <QDebug>

Likelihood::Likelihood()
  : m_type()
  , m_particles(Enums::NoParticle)
  , m_nodes()
{
}

Likelihood::~Likelihood()
{
}

void Likelihood::setType(Enums::LikelihoodVariable type)
{
  m_type = type;
  loadNodes();
}

Enums::LikelihoodVariable Likelihood::type() const
{
  return m_type;
}

Likelihood::MomentumMap::ConstIterator Likelihood::end(Enums::Particle type) const
{
  return m_nodes[type].end();
}

Likelihood::MomentumMap::ConstIterator Likelihood::lowerNode(Enums::Particle type, double p) const
{
  ParticleMap::ConstIterator particleIt = m_nodes.find(type);
  MomentumMap::ConstIterator momentumIt = particleIt.value().lowerBound(p);
  if (qFuzzyCompare(momentumIt.key(), p))
    return momentumIt;
  return --momentumIt;
}

Likelihood::MomentumMap::ConstIterator Likelihood::upperNode(Enums::Particle type, double p) const
{
  return ++lowerNode(type, p);
}

Likelihood::ParameterVector Likelihood::linearInterpolation(Enums::Particle particle, double p, bool* goodInterpolation) const
{
  MomentumMap::ConstIterator endIt = end(particle);
  MomentumMap::ConstIterator l = lowerNode(particle, p);
  if (qFuzzyCompare(l.key(), p))
    return l.value();
  MomentumMap::ConstIterator u = l;
  ++u;
  if (l == endIt || u == endIt) {
    if (goodInterpolation) {
      *goodInterpolation = false;
    } else {
      qWarning()
        << "No values for an interpolation of" << Enums::label(type())
        << "for" << Enums::label(particle) << "at p =" << p << "GeV.";
    }
    return defaultParameters();
  }
  if (goodInterpolation)
    *goodInterpolation = true;
  double k = (p - l.key()) / (u.key() - l.key());
  ParameterVector vector(numberOfParameters());
  for (int i = 0; i < numberOfParameters(); ++i)
    vector[i] = l.value()[i] + k * (u.value()[i] - l.value()[i]);
  return vector;
}

Enums::Particles Likelihood::particles() const
{
  return m_particles;
}

void Likelihood::loadNodes()
{
  QString fileName = Helpers::analysisPath() + "/conf/Likelihood.conf";
  Q_ASSERT(QFile::exists(fileName));
  QSettings settings(fileName, QSettings::IniFormat);
  settings.beginGroup(Enums::label(m_type));
  
  foreach (QString particleKey, settings.childGroups()) {
    settings.beginGroup(particleKey);
    Enums::Particle particle = Enums::particle(particleKey);
    m_particles|= particle;
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

double Likelihood::ratio(double p, Enums::Particle particle, double realMomentum) const
{
  double signalHypothesis = eval(p, particle, realMomentum);
  if (qIsNull(signalHypothesis))
    return 0;
  double backgroundHypothesis = 0;
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it)
    if ((it.key() & particles()) && (it.key() != particle))
      backgroundHypothesis+= eval(p, it.key(), realMomentum);
  if (qIsNull(backgroundHypothesis))
    return 0;
  return signalHypothesis / backgroundHypothesis;
}

LikelihoodPDF* Likelihood::pdf(Enums::Particle particle, double momentum) const
{
  return new LikelihoodPDF(this, particle, momentum);
}

LikelihoodRatio* Likelihood::ratio(Enums::Particle particle, double momentum) const
{
  return new LikelihoodRatio(this, particle, momentum);
}

Likelihood* Likelihood::newLikelihood(Enums::LikelihoodVariable type)
{
  switch (type) {
    case Enums::SignalHeightTrackerLikelihood: return 0;
    case Enums::SignalHeightTRDLikelihood: return 0;
    case Enums::TimeOverThresholdLikelihood: return 0;
    case Enums::TimeOfFlightLikelihood: return new TimeOfFlightLikelihood;
    case Enums::TrackerMomentumLikelihood: return new TrackerMomentumLikelihood;
  }
  return 0;
}

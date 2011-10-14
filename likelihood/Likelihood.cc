#include "Likelihood.hh"
#include "Helpers.hh"
#include "LikelihoodPDF.hh"
#include "LikelihoodRatio.hh"

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

QVector<double> Likelihood::linearInterpolation(Enums::Particle particle, double momentum) const
{
  MomentumMap::ConstIterator endIt = end(particle);
  MomentumMap::ConstIterator l = lowerNode(particle, momentum);
  if (qFuzzyCompare(l.key(), momentum))
    return l.value();
  MomentumMap::ConstIterator u = l;
  ++u;
  if (l == endIt || u == endIt)
    return QVector<double>();
  QVector<double> vector(numberOfParameters());
  double k = (momentum - l.key()) / (u.key() - l.key());
  for (int i = 0; i < numberOfParameters(); ++i) {
    vector[i] = l.value()[i] + k * (u.value()[i] - l.value()[i]);
  }
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
        momentumIt = particleIt.value().insert(p, QVector<double>(variantList.size()));
      for (int i = 0; i < variantList.size(); ++i)
        momentumIt.value()[i] = variantList[i].toDouble();
    }
    settings.endGroup();
  }
  settings.endGroup();
}

double Likelihood::ratio(double p, Enums::Particle particle, double reconstructedMomentum) const
{
  double conditionalProbability = eval(p, particle, reconstructedMomentum);
  if (qIsNull(conditionalProbability))
    return 0;
  double totalProbability = 0;
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it)
    if (it.key() & particles())
      totalProbability+= eval(p, it.key(), reconstructedMomentum);
  if (qIsNull(totalProbability))
    return 0;
  return eval(p, particle, reconstructedMomentum) / totalProbability;
}

LikelihoodPDF* Likelihood::pdf(Enums::Particle particle, double momentum) const
{
  return new LikelihoodPDF(this, particle, momentum, min(), max());
}

LikelihoodRatio* Likelihood::ratio(Enums::Particle particle, double momentum) const
{
  return new LikelihoodRatio(this, particle, momentum, min(), max());
}

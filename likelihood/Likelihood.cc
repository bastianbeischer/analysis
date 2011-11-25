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

Likelihood::Likelihood(Enums::Particles particles)
  : m_particles(particles)
  , m_measuredValueType(Enums::UndefinedKineticVariable)
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

Likelihood::AbsoluteRigidityMap::ConstIterator Likelihood::end(Enums::Particle type) const
{
  return m_nodes[type].end();
}

Likelihood::AbsoluteRigidityMap::ConstIterator Likelihood::lowerNode(const Hypothesis& hypothesis) const
{
  ParticleMap::ConstIterator particleIt = m_nodes.find(hypothesis.particle());
  if (particleIt == m_nodes.end()) {
    qDebug() << "No lower node for hypothesis" << hypothesis << "found.";
    Q_ASSERT(false);
  }
  Q_ASSERT_X(particleIt.value().count(), "Likelihood::lowerNode()", "No momenta found for requested particle species.");
  AbsoluteRigidityMap::ConstIterator momentumIt = particleIt.value().lowerBound(hypothesis.absoluteRigidity());
  //Q_ASSERT(momentumIt != particleIt.value().end());
  if (qFuzzyCompare(momentumIt.key(), hypothesis.absoluteRigidity()))
    return momentumIt;
  return --momentumIt;
}

Likelihood::AbsoluteRigidityMap::ConstIterator Likelihood::upperNode(const Hypothesis& hypothesis) const
{
  return ++lowerNode(hypothesis);
}

Likelihood::ParameterVector Likelihood::interpolation(const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  // linear interpolation method. Other methods should be implemented in the
  // according class that inherits Likelihood.
  AbsoluteRigidityMap::ConstIterator endIt = end(hypothesis.particle());
  AbsoluteRigidityMap::ConstIterator l = lowerNode(hypothesis);
  if (qFuzzyCompare(l.key(), hypothesis.absoluteRigidity()))
    return l.value();
  AbsoluteRigidityMap::ConstIterator u = l;
  ++u;
  if (goodInterpolation)
    *goodInterpolation = (l != endIt) && (u != endIt);
  if (l == endIt)
    return u.value();
  if (u == endIt)
    return l.value();
  double k = (hypothesis.absoluteRigidity() - l.key()) / (u.key() - l.key());
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
  /*Hypothesis h(Enums::Electron, -1./2.3);
  qDebug() << h;
  if (lowerNode(h) == end(h.particle())) qDebug() << "lower end";
  else qDebug() << "lower" << *lowerNode(h);
  if (upperNode(h) == end(h.particle())) qDebug() << "upper end";
  else qDebug() << "upper" << *upperNode(h);*/
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

#include "SignalHeightTrdLikelihood.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"
#include "TRDReconstruction.hh"
#include "Helpers.hh"

#include <TMath.h>

#include <QDebug>
#include <QFile>
#include <QSettings>

SignalHeightTrdLikelihood::SignalHeightTrdLikelihood(Enums::Particles particles)
  : Likelihood(particles)
  , m_layer(-1)
{
  m_likelihoodVariableType = Enums::SignalHeightTrdLikelihood;
  m_measuredValueType = Enums::UndefinedKineticVariable;
  m_min = -1.; //keV
  m_max = 11.; //keV
  m_numberOfParameters = 9;
  m_title = Enums::label(m_likelihoodVariableType) + "undefined layer";
}

int SignalHeightTrdLikelihood::layer() const
{
  return m_layer;
}

void SignalHeightTrdLikelihood::setLayer(int layer)
{
  Q_ASSERT(0 <= layer && layer <= 7);
  m_layer = layer;
  m_title = QString("%1 layer %2").arg(Enums::label(m_likelihoodVariableType)).arg(m_layer);
  loadNodes();
}

double SignalHeightTrdLikelihood::noTransitionRadiation(double x, const ParameterVector& parameterVector) const
{
  Q_ASSERT(m_numberOfParameters == parameterVector.count());
  const double* p = parameterVector.constData();
  //p[0] normalization factor
  //p[1] mopv
  //p[2] sigma of rising landau flank
  //p[3] sigma of falling landau flank
  //p[4] linear term in exponential
  //p[5] quadratic term in exponential
  //p[6] cubic term in exponential
  //p[7] const for gauss
  //p[8] sigma of gauss
  double polynom = p[4] * x + p[5] * x*x + p[6] * x*x*x;
  double sigma = x < p[1] ? p[2] : p[3];
  double result = p[0] * TMath::Landau(x, p[1], sigma) * TMath::Exp(polynom) + p[7] * TMath::Gaus(x, 0., p[8]);
  if (result < -100. || result > 100.)
    return 0;
  return result;
}

double SignalHeightTrdLikelihood::transitionRadiation(double x, const ParameterVector& parameterVector) const
{
  Q_ASSERT(m_numberOfParameters == parameterVector.count());
  const double* p = parameterVector.constData();
  double leftLandau = p[0] * TMath::Landau(x, p[1], p[2]);
  double rightLandau = p[3] * TMath::Landau(x, p[4], p[5]);
  double exponential = TMath::Exp(p[6] + p[7] * x);
  //p[8] is not used.
  double result = (leftLandau + rightLandau) * exponential;
  if (result < -100. || result > 100.)
    return 0;
  return result;
}

SignalHeightTrdLikelihood::~SignalHeightTrdLikelihood()
{
}

double SignalHeightTrdLikelihood::eval(const AnalyzedEvent* event, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  Q_ASSERT(0 <= m_layer && m_layer <= 7);
  const EnergyDeposition& deposition = event->particle()->trdReconstruction()->energyDepositionForLayer(m_layer);
  double signal = deposition.edepOnTrackPerLength;
  return eval(signal, hypothesis, goodInterpolation);
}

double SignalHeightTrdLikelihood::eval(double signal, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  Q_ASSERT_X(0 <= m_layer && m_layer <= 7, "SignalHeightTrdLikelihood::eval()", "call setLayer() before evaluating the PDF!");
  double normalization = 0;
  const ParameterVector& parameters = interpolation(hypothesis, normalization, goodInterpolation);
  Enums::Particle particle = hypothesis.particle();
  if (particle == Enums::Electron || particle == Enums::Positron)
    return normalization * transitionRadiation(signal, parameters);
  return normalization * noTransitionRadiation(signal, parameters);
}

void SignalHeightTrdLikelihood::loadNodes()
{
  QString fileName = Helpers::analysisPath() + "/conf/Likelihood.conf";
  Q_ASSERT(QFile::exists(fileName));
  QSettings settings(fileName, QSettings::IniFormat);
  m_nodes.clear();
  settings.beginGroup(Enums::label(m_likelihoodVariableType));
  Enums::Particles particlesInFile = Enums::particles(settings.value("particles").toString());
  Enums::ParticleIterator particleEnd = Enums::particleEnd();
  for (Enums::ParticleIterator particleIt = Enums::particleBegin(); particleIt != particleEnd; ++particleIt) {
    if (particleIt.key() == Enums::NoParticle || !(particleIt.key() & m_particles) || !(particleIt.key() & particlesInFile))
      continue;
    Q_ASSERT(particleIt.key() & m_particles);
    bool trParticle = (particleIt.key() == Enums::Electron) || (particleIt.key() == Enums::Positron);
    ParticleMap::Iterator particleNodeIterator = m_nodes.find(particleIt.key());
    if (particleNodeIterator == m_nodes.end())
      particleNodeIterator = m_nodes.insert(particleIt.key(), AbsoluteRigidityMap());
    settings.beginGroup(particleIt.value());

    QString postFix;
    if (trParticle)
      postFix+= QString("/layer%1").arg(m_layer);
    QList<QVariant> normalizationRrigiditiesVariantList = settings.value("normalizationRigidities" + postFix).toList();
    QList<QVariant> normalizationFactorsVariantList = settings.value("normalizationFactors" + postFix).toList();
    Q_ASSERT(normalizationRrigiditiesVariantList.size() == normalizationFactorsVariantList.size());
    NormalizationMap normalizationMap;
    for (int i = 0; i < normalizationFactorsVariantList.size(); ++i)
      normalizationMap.insert(normalizationRrigiditiesVariantList[i].toDouble(), normalizationFactorsVariantList[i].toDouble());
    m_normalization.insert(particleIt.key(), normalizationMap);

    QList<QVariant> rigiditiesVariantList = settings.value("absoluteRigidities").toList();
    foreach(QVariant rigidityVariant, rigiditiesVariantList) {
      double rigidity = rigidityVariant.toDouble();
      QString key = trParticle ? QString("%1GV/layer%2").arg(rigidityVariant.toString()).arg(m_layer)
        : QString("%1GV").arg(rigidityVariant.toString());
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

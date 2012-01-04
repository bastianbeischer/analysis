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
  m_measuredValueMin = -1.; //keV
  m_measuredValueMax = 11.; //keV
  m_numberOfParameters = 9;
  m_defaultParameters = PDFParameters(m_numberOfParameters);
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
  loadParameters();
}

double SignalHeightTrdLikelihood::noTransitionRadiation(double x, const PDFParameters& parameterVector) const
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

double SignalHeightTrdLikelihood::transitionRadiation(double x, const PDFParameters& parameterVector) const
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
  const PDFParameters& parameters = interpolation(hypothesis, goodInterpolation);
  Enums::Particle particle = hypothesis.particle();
  if (particle == Enums::Electron || particle == Enums::Positron)
    return parameters.normalizationFactor() * transitionRadiation(signal, parameters);
  return parameters.normalizationFactor() * noTransitionRadiation(signal, parameters);
}

void SignalHeightTrdLikelihood::loadParameters()
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
      QString prefix;
      if (particleIt.key() == Enums::Electron || particleIt.key() == Enums::Positron)
        prefix = QString("layer%1/").arg(m_layer);
      QVector<double> normalizationRigidities = Helpers::variantToDoubleVector(settings.value(prefix + "normalizationRigidities"));
      bool normalization = normalizationRigidities.count();
      Q_ASSERT(Helpers::sorted(normalizationRigidities));
      Q_ASSERT(!normalization || qFuzzyCompare(normalizationRigidities.first(), min));
      Q_ASSERT(!normalization || qFuzzyCompare(normalizationRigidities.last(), max));
      QVector<double> normalizationFactors = Helpers::variantToDoubleVector(settings.value(prefix + "normalizationFactors"));
      Q_ASSERT(normalizationRigidities.count() == normalizationFactors.count());
      for (int i = 0; i < numberOfElements; ++i) {
        double rigidity = min + i * m_parametersVectorsStep;
        double normalizationFactor = 1;
        if (normalization)
          normalizationFactor = interpolation(rigidity, normalizationRigidities, normalizationFactors);
        parametersVector[i].setNormalizationFactor(normalizationFactor);
        for (int parameter = 0; parameter < m_numberOfParameters; ++parameter) {
          QString key = QString::number(parameter);
          const QVector<double>& parameters = Helpers::variantToDoubleVector(settings.value(prefix + QString::number(parameter)));
          parametersVector[i][parameter] = interpolation(rigidity, rigidities, parameters);
        }
      }
      m_parametersVectors.insert(particleIt.key(), parametersVector);
    }
    settings.endGroup();
  }
  settings.endGroup();
}

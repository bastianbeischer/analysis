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
  PDFParameters parameters = PDFParameters()
    << 11.6699 << 0.2354 << 0.0734 << 0.1088 << -0.2164 << 0.0102 << -0.0004 << 0.1350 << 0.0052;
  m_defaultParameters = parameters;
  parameters = PDFParameters()
    << 1.8357 << 0.3923 << 0.1393 << 0.5024 << 2.0420 << 0.58549 << 1.1104 << -0.2729 << 0;
  m_defaultParametersMap.insert(Enums::Electron, parameters);
  m_defaultParametersMap.insert(Enums::Positron, parameters);
  m_title = Enums::label(m_likelihoodVariableType);
  if (!((m_particles & Enums::Electron) || (m_particles & Enums::Positron)))
    loadParameters();
}

int SignalHeightTrdLikelihood::layer() const
{
  return m_layer;
}

void SignalHeightTrdLikelihood::setLayer(int layer)
{
  Q_ASSERT(!((m_particles & Enums::Electron) || (m_particles & Enums::Positron)) || (0 <= layer && layer <= 7));
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
  Enums::Particle particle = hypothesis.particle();
  Q_ASSERT_X((particle != Enums::Electron && particle != Enums::Positron)
   || (0 <= m_layer && m_layer <= 7), "SignalHeightTrdLikelihood::eval()", "call setLayer() before evaluating the PDF!");
  const EnergyDeposition& deposition = event->particle()->trdReconstruction()->energyDepositionForLayer(m_layer);
  double signal = deposition.edepOnTrackPerLength;
  return eval(signal, hypothesis, goodInterpolation);
}

double SignalHeightTrdLikelihood::eval(double signal, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  Enums::Particle particle = hypothesis.particle();
  Q_ASSERT_X((particle != Enums::Electron && particle != Enums::Positron)
   || (0 <= m_layer && m_layer <= 7), "SignalHeightTrdLikelihood::eval()", "call setLayer() before evaluating the PDF!");
  const PDFParameters& parameters = interpolation(hypothesis, goodInterpolation);
  return eval(signal, hypothesis, parameters);
}

double SignalHeightTrdLikelihood::eval(double signal, const Hypothesis& hypothesis, const PDFParameters& parameters) const
{
  Enums::Particle particle = hypothesis.particle();
  Q_ASSERT_X((particle != Enums::Electron && particle != Enums::Positron )
    || (0 <= m_layer && m_layer <= 7), "SignalHeightTrdLikelihood::eval()", "call setLayer() before evaluating the PDF!");
  if (particle == Enums::Electron || particle == Enums::Positron)
    return parameters.normalizationFactor() * transitionRadiation(signal, parameters);
  return parameters.normalizationFactor() * noTransitionRadiation(signal, parameters);
}

void SignalHeightTrdLikelihood::saveParameters(const Hypothesis& h, const PDFParameters& parameters)
{
  QString fileName = Helpers::analysisPath() + "/conf/Likelihood.conf";
  Q_ASSERT(QFile::exists(fileName));
  QSettings settings(fileName, QSettings::IniFormat);

  settings.beginGroup(Enums::label(m_likelihoodVariableType));
  settings.beginGroup(Enums::label(h.particle()));

  QString prefix;
  if (h.particle() == Enums::Electron || h.particle() == Enums::Positron)
    prefix = QString("layer%1/").arg(m_layer);

  typedef QVector<double> DoubleVector;
  DoubleVector rigidities = Helpers::variantToDoubleVector(settings.value(prefix + "absoluteRigidities"));
  QVector<DoubleVector> values = QVector<DoubleVector>(m_numberOfParameters);
  for (int parameter = 0; parameter < m_numberOfParameters; ++parameter)
    values[parameter] = Helpers::variantToDoubleVector(settings.value(prefix + QString::number(parameter)));

  for (int i = 0; i < rigidities.size(); ++i) {
    if (qFuzzyCompare(h.absoluteRigidity(), rigidities[i])) {
      rigidities.remove(i);
      for (int parameter = 0; parameter < m_numberOfParameters; ++parameter)
        values[parameter].remove(i);
    }
  }

  int position = 0;
  while (position < rigidities.size() && rigidities[position] < h.absoluteRigidity())
    ++position;

  rigidities.insert(position, h.absoluteRigidity());
  Q_ASSERT(Helpers::sorted(rigidities));
  settings.setValue(prefix + "absoluteRigidities", Helpers::doubleVectorToVariant(rigidities));
  for (int parameter = 0; parameter < m_numberOfParameters; ++parameter) {
    values[parameter].insert(position, parameters[parameter]);
    settings.setValue(prefix + QString::number(parameter), Helpers::doubleVectorToVariant(values[parameter]));
  }

  settings.endGroup();
  settings.endGroup();
  settings.sync();

  loadParameters();
}

void SignalHeightTrdLikelihood::saveNormalization(Enums::Particle particle, const QVector<double>& rigidities, const QVector<double>& normalizationFactors)
{
  QString fileName = Helpers::analysisPath() + "/conf/Likelihood.conf";
  Q_ASSERT(QFile::exists(fileName));
  QSettings settings(fileName, QSettings::IniFormat);
  Q_ASSERT(rigidities.count() == normalizationFactors.count());
  settings.beginGroup(Enums::label(m_likelihoodVariableType));
  settings.beginGroup(Enums::label(particle));

  QString prefix;
  if (particle == Enums::Electron || particle == Enums::Positron)
    prefix = QString("layer%1/").arg(m_layer);
  settings.setValue(prefix + "normalizationRigidities", Helpers::doubleVectorToVariant(rigidities));
  settings.setValue(prefix + "normalizationFactors", Helpers::doubleVectorToVariant(normalizationFactors));

  settings.endGroup();
  settings.endGroup();
  settings.sync();

  loadParameters();
}

void SignalHeightTrdLikelihood::loadParameters()
{
  QString fileName = Helpers::analysisPath() + "/conf/Likelihood.conf";
  Q_ASSERT(QFile::exists(fileName));
  QSettings settings(fileName, QSettings::IniFormat);
  m_parametersVectors.clear();
  m_parametrizationMin = 0;
  m_parametrizationMax = 0;
  m_buffer = Buffer(Enums::NoParticle, 0);
  settings.beginGroup(Enums::label(m_likelihoodVariableType));
  Enums::Particles particlesInFile = Enums::particles(settings.value("particles").toString());
  Enums::ParticleIterator particleEnd = Enums::particleEnd();

  for (Enums::ParticleIterator particleIt = Enums::particleBegin(); particleIt != particleEnd; ++particleIt) {
    if (particleIt.key() == Enums::NoParticle)
      continue;
    if ((particleIt.key() & particlesInFile) != particleIt.key())
      continue;
    if ((particleIt.key() & m_particles) != particleIt.key())
      continue;
    settings.beginGroup(particleIt.value());
    QString prefix;
    if (particleIt.key() == Enums::Electron || particleIt.key() == Enums::Positron)
      prefix = QString("layer%1/").arg(m_layer);
    const QVector<double>& rigidities = Helpers::variantToDoubleVector(settings.value(prefix + "absoluteRigidities"));
    Q_ASSERT(Helpers::sorted(rigidities));
    if (rigidities.size()) {
      m_parametrizationMin = rigidities.first();
      m_parametrizationMax = rigidities.last();
      int numberOfElements = (m_parametrizationMax - m_parametrizationMin) / m_parametrizationStep + 1;
      PDFParametersVector parametersVector(numberOfElements, m_numberOfParameters);
      parametersVector.setRange(m_parametrizationMin, m_parametrizationMax);
      QVector<double> normalizationRigidities = Helpers::variantToDoubleVector(settings.value(prefix + "normalizationRigidities"));
      bool normalization = normalizationRigidities.count();
      Q_ASSERT(Helpers::sorted(normalizationRigidities));
      Q_ASSERT(!normalization || qFuzzyCompare(normalizationRigidities.first(), m_parametrizationMin));
      Q_ASSERT(!normalization || qFuzzyCompare(normalizationRigidities.last(), m_parametrizationMax));
      QVector<double> normalizationFactors = Helpers::variantToDoubleVector(settings.value(prefix + "normalizationFactors"));
      Q_ASSERT(normalizationRigidities.count() == normalizationFactors.count());
      for (int i = 0; i < numberOfElements; ++i) {
        double rigidity = m_parametrizationMin + i * m_parametrizationStep;
        if (i == numberOfElements - 1) // necessary to prevent rounding error
          rigidity = m_parametrizationMax;
        double normalizationFactor = 1.;
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

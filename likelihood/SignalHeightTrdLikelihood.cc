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
  m_measuredValueAxisTitle = "TRD signal / keV";
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

QString SignalHeightTrdLikelihood::particlePrefix(Enums::Particle particle) const
{
  if (particle == Enums::Electron || particle == Enums::Positron) {
    Q_ASSERT((0 <= m_layer && m_layer <= 7));
    return QString("layer%1/").arg(m_layer);
  }
  return QString();
}

#include "TimeOverThresholdLikelihood.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"
#include "TimeOfFlight.hh"
#include "Helpers.hh"

#include <TMath.h>

#include <QDebug>

TimeOverThresholdLikelihood::TimeOverThresholdLikelihood(Enums::Particles particles)
  : Likelihood(particles)
{
  m_likelihoodVariableType = Enums::TimeOverThresholdLikelihood;
  m_measuredValueType = Enums::UndefinedKineticVariable;
  m_measuredValueMin = 0.;
  m_measuredValueMax = 60.;
  m_numberOfParameters = 2;
  m_defaultParameters = PDFParameters() << 30.0 << 1.0;
  m_title = Enums::label(m_likelihoodVariableType);
  loadParameters();
}

TimeOverThresholdLikelihood::~TimeOverThresholdLikelihood()
{
}

double TimeOverThresholdLikelihood::eval(const AnalyzedEvent* event, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  double signal = event->particle()->timeOfFlight()->timeOverThreshold();
  return eval(signal, hypothesis, goodInterpolation);
}

double TimeOverThresholdLikelihood::eval(double timeOverThreshold, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  const PDFParameters& parameters = interpolation(hypothesis, goodInterpolation);
  return TMath::Gaus(timeOverThreshold, parameters[0], parameters[1]);
}

double TimeOverThresholdLikelihood::eval(double timeOverThreshold, const Hypothesis&, const PDFParameters& parameters) const
{
  return TMath::Gaus(timeOverThreshold, parameters[0], parameters[1], true);
}

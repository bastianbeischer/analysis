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
  m_min = 0.;
  m_max = 60.;
  m_numberOfParameters = 2;
  m_title = Enums::label(m_likelihoodVariableType);
}

TimeOverThresholdLikelihood::~TimeOverThresholdLikelihood()
{
}

double TimeOverThresholdLikelihood::eval(const AnalyzedEvent* event, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  double signal = event->particle()->timeOfFlight()->timeOverThreshold();
  return eval(signal, hypothesis, goodInterpolation);
}

double TimeOverThresholdLikelihood::eval(double signal, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  double normalization = 0;
  const ParameterVector& parameters = interpolation(hypothesis, normalization, goodInterpolation);
  return TMath::Gaus(signal, parameters[0], parameters[1]);
}

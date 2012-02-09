#include "TimeOfFlightLikelihood.hh"
#include "TimeOfFlight.hh"
#include "Helpers.hh"
#include "Constants.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"

#include <TMath.h>

#include <QDebug>

TimeOfFlightLikelihood::TimeOfFlightLikelihood(Enums::Particles particles)
  : Likelihood(particles)
{
  m_likelihoodVariableType = Enums::TimeOfFlightLikelihood;
  m_measuredValueAxisTitle = "#beta^{-1}";
  m_measuredValueMin = 0.;
  m_measuredValueMax = 5.;
  m_numberOfParameters = 0;
  m_title = Enums::label(m_likelihoodVariableType);
}

TimeOfFlightLikelihood::~TimeOfFlightLikelihood()
{
}

double TimeOfFlightLikelihood::eval(const AnalyzedEvent* event, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  double beta = event->particle()->timeOfFlight()->beta();
  if (qIsNull(beta))
    return 0;
  return eval(1./beta, hypothesis, goodInterpolation);
}

double TimeOfFlightLikelihood::eval(double inverseBeta, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  if (goodInterpolation)
    *goodInterpolation = true;
  return eval(inverseBeta, hypothesis, m_defaultParameters);
}

double TimeOfFlightLikelihood::eval(double inverseBeta, const Hypothesis& hypothesis, const PDFParameters&) const
{
  if (inverseBeta < m_measuredValueMin || inverseBeta > m_measuredValueMax)
    return 0;
  const double timeResolution = 0.4;
  const double length = Constants::upperTofPosition - Constants::lowerTofPosition;
  const double invBetaResolution = Constants::speedOfLight * timeResolution / length;
  return TMath::Gaus(inverseBeta, hypothesis.inverseBeta(), invBetaResolution, true);
}

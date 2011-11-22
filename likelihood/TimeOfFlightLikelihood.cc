#include "TimeOfFlightLikelihood.hh"
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
  m_measuredValueType = Enums::InverseBeta;
  m_min = 0.;
  m_max = 5.;
  m_numberOfParameters = 0;
}

TimeOfFlightLikelihood::~TimeOfFlightLikelihood()
{
}

double TimeOfFlightLikelihood::eval(const AnalyzedEvent* event, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  if (qIsNull(event->particle()->beta()))
    return 0;
  return eval(1./event->particle()->beta(), hypothesis, goodInterpolation);
}

double TimeOfFlightLikelihood::eval(double inverseBeta, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  if (goodInterpolation)
    *goodInterpolation = true;
  if (inverseBeta < m_min || inverseBeta > m_max)
      return 0;
  double timeResolution = 0.4;
  double length = Constants::upperTofPosition - Constants::lowerTofPosition;
  double invBetaResolution = Constants::speedOfLight * timeResolution / length;
  return TMath::Gaus(inverseBeta, hypothesis.inverseBeta(), invBetaResolution, true);
}

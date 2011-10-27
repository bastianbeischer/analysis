#include "TimeOfFlightLikelihood.hh"
#include "Helpers.hh"
#include "Constants.hh"
#include "Particle.hh"

#include <TMath.h>

#include <QDebug>

TimeOfFlightLikelihood::TimeOfFlightLikelihood()
  : Likelihood()
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

double TimeOfFlightLikelihood::eval(const Particle* particle, const KineticVariable& hypothesis, bool* goodInterpolation) const
{
  return eval(qIsNull(particle->beta()) ? 0. : 1./particle->beta(), hypothesis, goodInterpolation);
}

double TimeOfFlightLikelihood::eval(double inverseBeta, const KineticVariable& hypothesis, bool* goodInterpolation) const
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

#include "TimeOfFlightLikelihood.hh"
#include "Helpers.hh"
#include "Constants.hh"
#include "Particle.hh"

#include <TMath.h>

#include <QDebug>

TimeOfFlightLikelihood::TimeOfFlightLikelihood()
  : Likelihood()
{
  setType(Enums::TimeOfFlightLikelihood);
}

TimeOfFlightLikelihood::~TimeOfFlightLikelihood()
{
}

double TimeOfFlightLikelihood::min() const
{
  return 0.;
}

double TimeOfFlightLikelihood::max() const
{
  return 100.;
}

int TimeOfFlightLikelihood::numberOfParameters() const
{
  return 1;
}

double TimeOfFlightLikelihood::eval(double p, Enums::Particle particle, double realMomentum) const
{
  if (p < min() || p > max())
    return 0;
  double mass = Particle(particle).mass();
  double timeResolution = 0.4;
  double length = Constants::upperTofPosition - Constants::lowerTofPosition;
  double invBetaResolution = Constants::speedOfLight * timeResolution / length;

  double realInvBeta = Helpers::addQuad(mass, realMomentum) / realMomentum;
  QVector<double> parameters = linearInterpolation(particle, realMomentum);
  Q_ASSERT(parameters.size());
  double area = linearInterpolation(particle, realMomentum).at(0);
  return TMath::Gaus(Helpers::addQuad(mass, p) / p, realInvBeta, invBetaResolution, true) / area;
}

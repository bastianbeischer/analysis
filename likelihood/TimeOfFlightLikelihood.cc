#include "TimeOfFlightLikelihood.hh"
#include "Helpers.hh"
#include "Constants.hh"
#include "Particle.hh"

#include <TMath.h>

#include <QDebug>

const double TimeOfFlightLikelihood::s_cutoffMomentum = 100.;

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
  double min = 0.0;
  double max = s_cutoffMomentum;
  double range = max - min;
  return min - 0.05 * range;
}

double TimeOfFlightLikelihood::max() const
{
  double min = 0.0;
  double max = s_cutoffMomentum;
  double range = max - min;
  return max + 0.05 * range;
}

int TimeOfFlightLikelihood::numberOfParameters() const
{
  return 1;
}

double TimeOfFlightLikelihood::eval(double p, Enums::Particle particle, double reconstructedMomentum) const
{
  if (p < 0 || p > s_cutoffMomentum)
    return 0;
  double mass = Particle(particle).mass();
  double timeResolution = 0.4;
  double length = Constants::upperTofPosition - Constants::lowerTofPosition;
  double invBetaResolution = Constants::speedOfLight * timeResolution / length;

  double reconstructedInvBeta = Helpers::addQuad(mass, reconstructedMomentum) / reconstructedMomentum;
  QVector<double> parameters = linearInterpolation(particle, reconstructedMomentum);
  Q_ASSERT(parameters.size());
  double area = linearInterpolation(particle, reconstructedMomentum).at(0);
  return TMath::Gaus(Helpers::addQuad(mass, p) / p, reconstructedInvBeta, invBetaResolution, true) / area;
}

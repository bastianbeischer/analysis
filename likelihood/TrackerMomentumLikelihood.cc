#include "TrackerMomentumLikelihood.hh"
#include "Helpers.hh"
#include "Constants.hh"
#include "Particle.hh"

#include <TMath.h>

#include <QDebug>

TrackerMomentumLikelihood::TrackerMomentumLikelihood()
  : Likelihood()
{
  setType(Enums::TrackerMomentumLikelihood);
}

TrackerMomentumLikelihood::~TrackerMomentumLikelihood()
{
}

double TrackerMomentumLikelihood::min() const
{
  return -100.;
}

double TrackerMomentumLikelihood::max() const
{
  return +100.;
}

int TrackerMomentumLikelihood::numberOfParameters() const
{
  return 1;
}

Likelihood::ParameterVector TrackerMomentumLikelihood::defaultParameters() const
{
  ParameterVector vector(numberOfParameters());
  vector[0] = 1.;
  return vector;
}

double TrackerMomentumLikelihood::eval(double p, Enums::Particle particle, double realMomentum, bool* goodInterpolation) const
{
  if (p < min() || p > max())
    return 0;
  double realMass = Particle(particle).mass();
  double realCharge = Particle(particle).charge();
  double realBeta = realMomentum / Helpers::addQuad(realMass, realMomentum);
  double realRigidity = realMomentum / realCharge;
  double realCurvature = 1. / realRigidity;

  double relativeSigma = Helpers::addQuad(0.08 * realRigidity, 0.25 / realBeta);
  
  double rigidity = p / qAbs(realCharge);
  double curvature = 1. / rigidity;
  double area = linearInterpolation(particle, realMomentum, goodInterpolation).at(0);
  //return TMath::Gaus(rigidity, realCharge*realCurvature, relativeSigma, true) / area;
  return TMath::Gaus(curvature, realCurvature, relativeSigma, true) / area;
}

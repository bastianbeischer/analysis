#include "CherenkovLikelihood.hh"
#include "Helpers.hh"
#include "Constants.hh"
#include "Particle.hh"
#include "Settings.hh"
#include "AnalyzedEvent.hh"
#include "SimpleEvent.hh"
#include "SensorTypes.hh"

#include <TMath.h>

#include <QDebug>

CherenkovLikelihood::CherenkovLikelihood(Enums::Particles particles)
  : Likelihood(particles)
  , m_numberOfParticles(0)
{
  m_likelihoodVariableType = Enums::CherenkovLikelihood;
  m_measuredValueType = Enums::UndefinedKineticVariable;
  m_min = 0.;
  m_max = 0.;
  m_numberOfParameters = 0;
  m_title = Enums::label(m_likelihoodVariableType);
  Enums::ParticleIterator end = Enums::particleEnd();
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != end; ++it)
    if (it.key() != Enums::NoParticle && m_particles & it.key())
      ++m_numberOfParticles;
}

CherenkovLikelihood::~CherenkovLikelihood()
{
}

double CherenkovLikelihood::eval(const AnalyzedEvent* event, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  // CherenkovLikelihood should not be used for non testbeam data.
  Q_ASSERT(event->settings()->situation() == Enums::Testbeam11);
  if (goodInterpolation)
    *goodInterpolation = true;
  int n;
  Enums::Particles particles = event->settings()->aboveThreshold(&n);
  if (n == m_numberOfParticles)
    return 1. / n;
  double c1 = event->simpleEvent()->sensorData(SensorTypes::BEAM_CHERENKOV1);
  double c2 = event->simpleEvent()->sensorData(SensorTypes::BEAM_CHERENKOV2);
  if (c1 > Constants::cherenkovThreshold && c2 > Constants::cherenkovThreshold && (hypothesis.particle() & particles))
    return 1. / n;
  if (c1 < Constants::cherenkovThreshold && c2 < Constants::cherenkovThreshold && !(hypothesis.particle() & particles))
    return 1. / (m_numberOfParticles - n);
  return 0;
}

double CherenkovLikelihood::eval(double, const Hypothesis&, bool*) const
{
  Q_ASSERT(false);
  return 0;
}

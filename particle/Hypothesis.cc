#include "Hypothesis.hh"
#include "Helpers.hh"
#include "ParticleProperties.hh"

#include <QDebug>

#include <cmath>

Hypothesis::Hypothesis(Enums::Particle particle, double curvature)
  : m_properties(particle)
  , m_curvature(curvature)
  , m_nLikelihoods(-1)
  , m_likelihood(-1)
{
}

Hypothesis::~Hypothesis()
{
}

Enums::Particle Hypothesis::particle() const
{
  return m_properties.type();
}

double Hypothesis::likelihood() const
{
  return m_likelihood;
}

int Hypothesis::numberOfLikelihoods() const
{
  return m_nLikelihoods;
}

void Hypothesis::setLikelihood(int n, double lh)
{
  m_nLikelihoods = n;
  m_likelihood = lh;
}

int Hypothesis::charge() const
{
  return m_properties.charge();
}

double Hypothesis::mass() const
{
  return m_properties.mass();
}

double Hypothesis::curvature() const
{
  return m_curvature;
}

double Hypothesis::rigidity() const
{
  return 1. / m_curvature;
}

double Hypothesis::absoluteRigidity() const
{
  double r = rigidity();
  if (r >= 0) {
    return r;
  } else {
    return -r;
  }
}

double Hypothesis::beta() const
{
  return 1. / inverseBeta();
}

double Hypothesis::inverseBeta() const
{
  double m = m_properties.mass();
  return sqrt(1. + m * m * m_curvature * m_curvature);
}

QDebug operator<<(QDebug debug, const Hypothesis& h)
{
  debug << QString("%1 (L %2) K=%3/GV R=%4GV beta=%5").arg(Enums::label(h.particle()))
    .arg(h.likelihood()).arg(h.curvature()).arg(h.rigidity()).arg(h.beta());
  return debug;
}

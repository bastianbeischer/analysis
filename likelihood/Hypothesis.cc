#include "Hypothesis.hh"
#include "Helpers.hh"
#include "ParticleProperties.hh"

#include <QDebug>

Hypothesis::Hypothesis(Enums::Particle particle, double curvature)
  : m_properties(particle)
  , m_curvature(curvature)
  , m_logLikelihood(-1)
{
}

Hypothesis::~Hypothesis()
{
}

Enums::Particle Hypothesis::particle() const
{
  return m_properties.type();
}

double Hypothesis::logLikelihood() const
{
  return m_logLikelihood;
}

void Hypothesis::setLogLikelihood(double llh)
{
  m_logLikelihood = llh;
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
  return qAbs(rigidity());
}

double Hypothesis::beta() const
{
  return 1. / inverseBeta();
}

double Hypothesis::inverseBeta() const
{
  return Helpers::addQuad(1., m_properties.mass() * m_curvature);
}

QDebug operator<<(QDebug debug, const Hypothesis& h)
{
  debug << QString("%1 (logL %2) K=%3/GV R=%4GV beta=%5").arg(Enums::label(h.particle()))
    .arg(h.logLikelihood()).arg(h.curvature()).arg(h.rigidity()).arg(h.beta());
  return debug;
}

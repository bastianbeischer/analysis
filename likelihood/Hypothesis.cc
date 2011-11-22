#include "Hypothesis.hh"
#include "Helpers.hh"
#include "ParticleProperties.hh"

#include <QDebug>

Hypothesis::Hypothesis(Enums::Particle particle, double curvature)
  : m_properties(particle)
  , m_curvature(curvature)
  , m_probability(-1)
{
}

Hypothesis::~Hypothesis()
{
}

Enums::Particle Hypothesis::particle() const
{
  return m_properties.type();
}

double Hypothesis::propability() const
{
  return m_probability;
}

void Hypothesis::setProbability(double p)
{
  Q_ASSERT(0 <= p && p <= 1);
  m_probability = p;
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

double Hypothesis::beta() const
{
  return 1. / inverseBeta();
}

double Hypothesis::inverseBeta() const
{
  return Helpers::addQuad(1., m_properties.mass() * m_curvature);
}

void Hypothesis::dump() const
{
  qDebug() << qPrintable(QString("%1 (probability %2) K=%3/GV R=%4GV beta=%5").arg(Enums::label(m_properties.type()))
    .arg(m_probability).arg(m_curvature).arg(rigidity()).arg(beta()));
}

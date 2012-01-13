#include "KineticVariable.hh"

#include <cmath>

#include <QDebug>

KineticVariable::KineticVariable(Enums::Particle particleType)
  : m_properties(particleType)
  , m_kineticEnergy(0)
{
}

KineticVariable::KineticVariable(Enums::Particle particleType, Enums::KineticVariable variableType, double value)
  : m_properties(particleType)
  , m_kineticEnergy(0)
{
  set(variableType, value);
}

const ParticleProperties& KineticVariable::properties() const
{
  return m_properties;
}

Enums::Particle KineticVariable::particle() const
{
  return m_properties.type();
}

int KineticVariable::charge() const
{
  return m_properties.charge();
}

double KineticVariable::mass() const
{
  return m_properties.mass();
}

double KineticVariable::totalEnergy() const
{
  return m_kineticEnergy + m_properties.mass();
}

double KineticVariable::kineticEnergy() const
{
  return m_kineticEnergy;
}

double KineticVariable::momentum() const
{
  return sqrt(totalEnergy() * totalEnergy() - m_properties.mass() * m_properties.mass());
}

double KineticVariable::rigidity() const
{
  return momentum() / m_properties.charge();
}

double KineticVariable::absoluteRigidity() const
{
  return qAbs(rigidity());
}

double KineticVariable::curvature() const
{
  return 1. / rigidity();
}

double KineticVariable::beta() const
{
  return momentum() / totalEnergy();
}

double KineticVariable::inverseBeta() const
{
  return totalEnergy() / momentum();
}

double KineticVariable::squaredCurvature() const
{
  double c = curvature();
  return c*c;
}

void KineticVariable::set(Enums::KineticVariable type, double value) {
  switch (type) {
    case Enums::UndefinedKineticVariable: qFatal("Bad KineticVariable type passed!"); break;
    case Enums::TotalEnergy: setTotalEnergy(value); break;
    case Enums::KineticEnergy: setKineticEnergy(value); break;
    case Enums::Momentum: setMomentum(value); break;
    case Enums::Rigidity: setRigidity(value); break;
    case Enums::AbsoluteRigidity: setAbsoluteRigidity(value); break;
    case Enums::Curvature: setCurvature(value); break;
    case Enums::Beta: setBeta(value); break;
    case Enums::InverseBeta: setInverseBeta(value); break;
    case Enums::SquaredCurvature: setCurvature(value); break;
  }
}

void KineticVariable::setTotalEnergy(double value)
{
  m_kineticEnergy = value - m_properties.mass();
}

void KineticVariable::setKineticEnergy(double value)
{
  m_kineticEnergy = value;
}

void KineticVariable::setMomentum(double value)
{
  setTotalEnergy(sqrt(value * value + m_properties.mass() * m_properties.mass()));
}

void KineticVariable::setRigidity(double value)
{
  double p = m_properties.charge() * value;
  Q_ASSERT(p >= 0); // incompatible charge sign
  setMomentum(p);
}

void KineticVariable::setAbsoluteRigidity(double value)
{
  Q_ASSERT(value >= 0);
  setRigidity(m_properties.charge() < 0 ? -value : value);
}

void KineticVariable::setCurvature(double value)
{
  setRigidity(1. / value);
}

void KineticVariable::setBeta(double value)
{
  setTotalEnergy(m_properties.mass() / sqrt(1. - value * value));
}

void KineticVariable::setInverseBeta(double value)
{
  setTotalEnergy(sqrt(1. - value * value) / m_properties.mass());
}

void KineticVariable::setSquaredCurvature(double value)
{
  Q_ASSERT(value >= 0);
  setCurvature(sqrt(value));
}

bool KineticVariable::operator<(const KineticVariable& other) const
{
  if (m_properties.type() < other.m_properties.type())
    return true;
  if (m_properties.type() == other.m_properties.type() && m_kineticEnergy < other.m_kineticEnergy)
    return true;
  return false;
}

QDebug operator<<(QDebug debug, const KineticVariable& variable)
{
  debug << QString("%1 K=%3/GV R=%4GV beta=%5").arg(Enums::label(variable.particle()))
    .arg(variable.curvature()).arg(variable.rigidity()).arg(variable.beta());
  return debug;
}

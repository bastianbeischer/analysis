#include "KineticVariable.hh"
#include "Particle.hh"

#include <cmath>

KineticVariable::KineticVariable(Enums::Particle particleType, double kineticEnergy)
  : m_charge(0)
  , m_mass(0)
  , m_kineticEnergy(kineticEnergy)
{
  Particle particle(particleType);
  m_mass = particle.mass();
  m_charge = particle.charge();
}

int KineticVariable::charge() const
{
  return m_charge;
}

double KineticVariable::mass() const
{
  return m_mass;
}

double KineticVariable::totalEnergy() const
{
  return m_kineticEnergy + m_mass;
}

double KineticVariable::kineticEnergy() const
{
  return m_kineticEnergy;
}

double KineticVariable::momentum() const
{
  return sqrt(totalEnergy() * totalEnergy() - m_mass * m_mass);
}

double KineticVariable::rigidity() const
{
  return momentum() / m_charge;
}

double KineticVariable::curvature() const
{
  return 1. / rigidity();
}

double KineticVariable::beta() const
{
  return momentum() / totalEnergy();
}

void KineticVariable::set(Type type, double value) {
  switch (type) {
    case TotalEnergy: setTotalEnergy(value); break;
    case KineticEnergy: setKineticEnergy(value); break;
    case Momentum: setMomentum(value); break;
    case Rigidity: setRigidity(value); break;
    case Curvature: setCurvature(value); break;
    case Beta: setBeta(value); break;
  }
}

void KineticVariable::setTotalEnergy(double value)
{
  m_kineticEnergy = value - m_mass;
}

void KineticVariable::setKineticEnergy(double value)
{
  m_kineticEnergy = value;
}

void KineticVariable::setMomentum(double value)
{
  setTotalEnergy(sqrt(value * value + m_mass * m_mass));
}

void KineticVariable::setRigidity(double value)
{
  setMomentum(m_charge * value);
}

void KineticVariable::setCurvature(double value)
{
  setRigidity(1. / value);
}

void KineticVariable::setBeta(double value)
{
  setTotalEnergy(m_mass / sqrt(1. - value * value));
}

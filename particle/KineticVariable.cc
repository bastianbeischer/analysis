#include "KineticVariable.hh"
#include "Particle.hh"

#include <cmath>

KineticVariable::KineticVariable(Enums::Particle particleType)
  : m_particle(particleType)
  , m_charge(0)
  , m_mass(0)
  , m_kineticEnergy(0)
{
  Particle particle(particleType);
  m_mass = particle.mass();
  m_charge = particle.charge();
}

KineticVariable::KineticVariable(Enums::Particle particleType, Enums::KineticVariable type, double value)
  : m_particle(particleType)
  , m_charge(0)
  , m_mass(0)
  , m_kineticEnergy(0)
{
  Particle particle(particleType);
  m_mass = particle.mass();
  m_charge = particle.charge();
  set(type, value);
}

Enums::Particle KineticVariable::particle() const
{
  return m_particle;
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

double KineticVariable::inverseBeta() const
{
  return totalEnergy() / momentum();
}

void KineticVariable::set(Enums::KineticVariable type, double value) {
  switch (type) {
    case Enums::UndefinedKineticVariable: qFatal("Bad KineticVariable type passed!"); break;
    case Enums::TotalEnergy: setTotalEnergy(value); break;
    case Enums::KineticEnergy: setKineticEnergy(value); break;
    case Enums::Momentum: setMomentum(value); break;
    case Enums::Rigidity: setRigidity(value); break;
    case Enums::Curvature: setCurvature(value); break;
    case Enums::Beta: setBeta(value); break;
    case Enums::InverseBeta: setInverseBeta(value); break;
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
  double p = m_charge * value;
  Q_ASSERT(p < 0); // incompatible charge sign
  setMomentum(p);
}

void KineticVariable::setCurvature(double value)
{
  setRigidity(1. / value);
}

void KineticVariable::setBeta(double value)
{
  setTotalEnergy(m_mass / sqrt(1. - value * value));
}

void KineticVariable::setInverseBeta(double value)
{
  setTotalEnergy(sqrt(1. - value * value) / m_mass);
}

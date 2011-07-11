#include "MagneticField.hh"

MagneticField::MagneticField() :
  m_z0(0.),
  m_z1(0.),
  m_fieldEstimate(0.),
  m_displacement(TVector3(0.,0.,0.))
{
}

MagneticField::~MagneticField()
{
}

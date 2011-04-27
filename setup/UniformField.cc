#include "UniformField.hh"

UniformField::UniformField(const TVector3& fieldVector) :
  MagneticField(),
  m_fieldVector(fieldVector)
{
  m_z0 = -40;
  m_z1 = 40;
  m_fieldEstimate = fieldVector.y();
}

UniformField::~UniformField()
{
}

void UniformField::fieldValue(const double* x, double* B) const
{
  double z = x[2] - m_displacement.z();

  B[0] = B[1] = B[2] = 0.;
  if ( (z > m_z0) && (z < m_z1) ) {
    B[0] = m_fieldVector.x();
    B[1] = m_fieldVector.y();
    B[2] = m_fieldVector.z();
  }
}

#ifndef MagneticField_hh
#define MagneticField_hh

#include <TVector3.h>

class Track;

class MagneticField
{
 
public:
  MagneticField();
  ~MagneticField();
 
public:
  void setZ0(const double& z0) {m_z0 = z0;}
  void setZ1(const double& z1) {m_z1 = z1;}
  void setFieldEstimate(const double& estimate) {m_fieldEstimate = estimate;}
  void setDisplacement(const TVector3& displacement) {m_displacement = displacement;}

  double z0() const {return m_z0;}
  double z1() const {return m_z1;}
  double fieldEstimate() const {return m_fieldEstimate;}
  const TVector3& displacement() const {return m_displacement;}

  virtual void fieldValue(const double* x, double* B) const = 0;

protected:
  double m_z0;
  double m_z1;
  double m_fieldEstimate;
  TVector3 m_displacement;

};

#endif /* MagneticField_hh */

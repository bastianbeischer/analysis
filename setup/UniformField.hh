#ifndef UniformField_hh
#define UniformField_hh

#include "MagneticField.hh"

class UniformField :
  public MagneticField
{
  
public:
  explicit UniformField(const TVector3&);
  ~UniformField();
  
  void fieldValue(const double* x, double* B) const;

private:
  const TVector3 m_fieldVector;
  
};

#endif /* UniformField_hh */

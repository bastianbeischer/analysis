#ifndef InhomField_hh
#define InhomField_hh

#include "MagneticField.hh"

class Axis;

class InhomField :
  public MagneticField
{
  
public:
  explicit InhomField(const char*);
  ~InhomField();
  
  void fieldValue(const double* x, double* B) const;

private:
  void readData();

private:
  const char* m_dataFileName;

  Axis* m_axis_x;
  Axis* m_axis_y;
  Axis* m_axis_z;
  double*** m_field_x;
  double*** m_field_y;
  double*** m_field_z;
  
};

#endif /* InhomField_hh */

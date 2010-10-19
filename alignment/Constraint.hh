#ifndef Constraint_hh
#define Constraint_hh

class Constraint
{

public:
  Constraint();
  ~Constraint();

public:
  void Activate();

private:
  float* m_listOfCoefficients;
  float  m_rhs;
  
};

#endif /* Constraint_hh */

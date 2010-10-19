#ifndef PEBS09_Constraint_h
#define PEBS09_Constraint_h

class PEBS09_Constraint
{

public:
  PEBS09_Constraint();
  ~PEBS09_Constraint();

public:
  void Activate();

private:
  float* m_listOfCoefficients;
  float  m_rhs;
  
};

#endif /* PEBS09_Constraint_h */

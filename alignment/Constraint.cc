#include "PEBS09_Constraint.h"

#include "millepede.h"

PEBS09_Constraint::PEBS09_Constraint() :
  m_listOfCoefficients(0),
  m_rhs(0.)
{
}

PEBS09_Constraint::~PEBS09_Constraint()
{
  delete [] m_listOfCoefficients;
}

void PEBS09_Constraint::Activate()
{
  CONSTF(m_listOfCoefficients, m_rhs);
}

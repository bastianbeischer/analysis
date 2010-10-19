#include "Constraint.hh"

#include "millepede.h"

Constraint::Constraint() :
  m_listOfCoefficients(0),
  m_rhs(0.)
{
}

Constraint::~Constraint()
{
  delete [] m_listOfCoefficients;
}

void Constraint::activate()
{
  CONSTF(m_listOfCoefficients, m_rhs);
}

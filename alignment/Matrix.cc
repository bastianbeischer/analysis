#include "Matrix.hh"

#include "Manager.hh"
#include "Strategy.hh"

Matrix::Matrix() :
  m_nGlobal(0),
  m_nLocal(0),
  m_globalDerivatives(0),
  m_localDerivatives(0)
{
}

Matrix::~Matrix()
{
  delete[] m_globalDerivatives;
  delete[] m_localDerivatives;
}

void Matrix::DoInit()
{
  Manager* manager = Manager::GetInstance();
  Strategy* strategy = manager->GetStrategy();

  m_nGlobal = strategy->GetNumberOfGlobalParameters();
  m_nLocal  = strategy->GetNumberOfLocalParameters();;

  delete[] m_globalDerivatives;
  delete[] m_localDerivatives;
  m_globalDerivatives = new float[m_nGlobal];
  m_localDerivatives = new float[m_nLocal];

  for (unsigned int i = 0; i < m_nGlobal; i++)
    m_globalDerivatives[i] = 0.;
  for (unsigned int i = 0; i < m_nLocal; i++)
    m_localDerivatives[i] = 0.;
}


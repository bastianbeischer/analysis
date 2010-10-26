#include "Parameters.hh"

#include "millepede.h"

#include "Setup.hh"
#include "DetectorElement.hh"

Parameters::Parameters() :
  m_nPar(0),
  m_parameters(0),
  m_parameterSigma(0)
{
}

Parameters::Parameters(unsigned int nPar)
{
  reInitParameterArrays(nPar);
}

Parameters::~Parameters()
{
  delete[] m_parameters;
  delete[] m_parameterSigma;
}

void Parameters::setMillepedeParameters()
{
  PARGLO(m_parameters);
  for (unsigned int i = 0; i < m_nPar; i++) {
    if (m_parameterSigma[i] >= 0.) {
      unsigned int iPar = i+1;
      PARSIG(iPar, m_parameterSigma[i]);
    }
  }
}

void Parameters::reInitParameterArrays(unsigned int nPar)
{
  m_nPar           = nPar;
  m_parameters     = new float[nPar];
  m_parameterSigma = new float[nPar];
  
  for (unsigned int i = 0; i < nPar; i++) {
    m_parameters[i] = 0.;
    // -1 means that this sigma is not used.
    m_parameterSigma[i] = -1.;
  }
}

// very lol.
unsigned int Parameters::indexForDetId(unsigned short detId) const
{
  Setup* setup = Setup::instance();
  int index = 0;
  DetectorElement* element = setup->firstElement();
  while(element) {
    if (element->id() == detId) break;
    element = setup->nextElement();
    index++;
  }
  return index;
}


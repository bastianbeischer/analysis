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
  detId &= 0xFF00;

  int index = 0;

  // TOF
  if (detId == 0x8000) index = 0;

  // Tracker e-side
  if (detId == 0x6100) index = 1;
  if (detId == 0x6400) index = 2;
  if (detId == 0x6000) index = 3;
  if (detId == 0x6200) index = 4;
  if (detId == 0x6500) index = 5;
  if (detId == 0x6700) index = 6;
  if (detId == 0x6600) index = 7;
  if (detId == 0x3300) index = 8;
  if (detId == 0x3700) index = 9;
  if (detId == 0x6300) index = 10;

  // Tracker g-side
  if (detId == 0x7B00) index = 11;
  if (detId == 0x7F00) index = 12;
  if (detId == 0x7A00) index = 13;
  if (detId == 0x7E00) index = 14;
  if (detId == 0x7900) index = 15;
  if (detId == 0x7D00) index = 16;
  if (detId == 0x7800) index = 17;
  if (detId == 0x7C00) index = 18;
  if (detId == 0x3100) index = 19;
  if (detId == 0x3000) index = 20;

  // TRD
  if (detId == 0x3200) index = 21;
  if (detId == 0x3600) index = 22;
  if (detId == 0x3400) index = 23;
  if (detId == 0x3500) index = 24;

  // Setup* setup = Setup::instance();
  // DetectorElement* element = setup->firstElement();
  // while(element) {
  //   if (element->id() == detId) break;
  //   element = setup->nextElement();
  //   index++;
  // }

  return index;
}

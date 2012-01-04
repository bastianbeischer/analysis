#include "Parameters.hh"

#include "millepede.h"

#include <iostream>

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

unsigned int Parameters::indexForDetId(unsigned short detId) const
{
  unsigned short connection = detId & 0xFF00;
  unsigned short hpo = (detId & 0x80) >> 7;
  unsigned short sipm = (detId & 0x60) >> 5;
  unsigned short module = (detId & 0x30) >> 4;

  unsigned short firstTrdIndex = 1;
  unsigned short numberOfUFEs = 4;
  unsigned short modulesPerUFE = 4;

  unsigned short firstTrackerIndex = firstTrdIndex + numberOfUFEs*modulesPerUFE;
  unsigned short numberOfHPEs = 20;
  unsigned short hposPerHPE = 2;
  unsigned short sipmsPerHpo = 4;

  unsigned short tofConnections[1]      = {0x8000};
  unsigned short trdConnections[4]      = {0x3200, 0x3600, 0x3400, 0x3500};
  unsigned short trackerConnections[20] = {0x6100, 0x6400, 0x6000, 0x6200, 0x6500, 0x6700, 0x6600, 0x3300, 0x3700, 0x6300,
                                           0x7B00, 0x7F00, 0x7A00, 0x7E00, 0x7900, 0x7D00, 0x7800, 0x7C00, 0x3100, 0x3000};

  // TOF
  if (connection == tofConnections[0]) return 0;

  for (int i = 0; i < numberOfUFEs; i++)
    if (connection == trdConnections[i]) return firstTrdIndex + i*modulesPerUFE + module;

  for (int i = 0; i < numberOfHPEs; i++)
    if (connection == trackerConnections[i]) return firstTrackerIndex + i*hposPerHPE*sipmsPerHpo + hpo*sipmsPerHpo + sipm;

  Q_ASSERT(false);
  return 0;
}

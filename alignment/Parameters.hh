#ifndef Parameters_hh
#define Parameters_hh

#include <cassert>
#include <QMap>
#include <QPair>

class Parameters
{

public:
  Parameters();
  Parameters(unsigned int nPar);
  ~Parameters();

public:
  void         setMillepedeParameters();
  void         reInitParameterArrays(unsigned int nPar);

  void         setParameter(unsigned int i, float value)      {assert(i < m_nPar); m_parameters[i] = value;}
  void         setParameterSigma(unsigned int i, float sigma) {assert(i < m_nPar); m_parameterSigma[i] = sigma;}

  float*       parameterArray()                     {return m_parameters;}
  float        parameter(unsigned int i)      const {assert(i < m_nPar); return m_parameters[i];}
  float        parameterSigma(unsigned int i) const {assert(i < m_nPar); return m_parameterSigma[i];}
  unsigned int numberOfParameters()           const {return m_nPar;}

private:
  
  unsigned int m_nPar;
  float*       m_parameters;
  float*       m_parameterSigma;

};

#endif /* Parameters_hh */

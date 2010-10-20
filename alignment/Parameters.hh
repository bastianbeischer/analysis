#ifndef Parameters_hh
#define Parameters_hh

#include <cassert>
#include <QMap>
#include <QPair>

class QFile;

typedef QPair<int,int> ArrayPair;

class Parameters
{

public:
  Parameters();
  Parameters(unsigned int nPar, unsigned int nAngles);
  ~Parameters();

public:
  void         setMillepedeParameters();
  void         reInitParameterArrays(unsigned int nPar, unsigned int nAngles);

  void         setParameter(unsigned int i, float value)      {assert(i < m_nPar); m_parameters[i] = value;}
  void         setParameterSigma(unsigned int i, float sigma) {assert(i < m_nPar); m_parameterSigma[i] = sigma;}
  void         setAngle(unsigned int i, float angle);

  float*       parameterArray()                     {return m_parameters;}
  float        parameter(unsigned int i)      const {assert(i < m_nPar); return m_parameters[i];}
  float        parameterSigma(unsigned int i) const {assert(i < m_nPar); return m_parameterSigma[i];}
  unsigned int numberOfParameters()           const {return m_nPar;}
  float        angle(unsigned int i)          const {assert(i < m_nAngles); return m_angles[i];}

  bool         writeToFile(QFile* file);

  unsigned int shiftParameterIndexForItem(unsigned int hybrid, unsigned int sipm);
  unsigned int rotationParameterIndexForItem(unsigned int uniqueLayer, unsigned int hybrid, unsigned int sipm);

private:
  void         constructIndexMap();

private:
  
  unsigned int m_nPar;
  unsigned int m_nAngles;
  float*       m_angles;
  float*       m_parameters;
  float*       m_parameterSigma;

  QMap<ArrayPair,int> m_indexMap;

};

#endif /* Parameters_hh */

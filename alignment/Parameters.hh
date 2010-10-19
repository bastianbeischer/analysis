#ifndef Parameters_hh
#define Parameters_hh

#include <cassert>
#include <map>
#include <utility>

class QFile;

typedef std::pair<int,int> ArrayPair;

class Parameters
{

public:
  Parameters();
  Parameters(unsigned int nPar, unsigned int nAngles);
  ~Parameters();

public:
  void  SetMillepedeParameters();
  void  ReInitParameterArrays(unsigned int nPar, unsigned int nAngles);

  void  SetParameter(unsigned int i, float value) {assert(i < m_nPar); m_parameters[i] = value;}
  void  SetParameterSigma(unsigned int i, float sigma) {assert(i < m_nPar); m_parameterSigma[i] = sigma;}
  void  SetAngle(unsigned int i, float angle);

  float*       GetParameterArray()                     {return m_parameters;}
  float        GetParameter(unsigned int i)      const {assert(i < m_nPar); return m_parameters[i];}
  float        GetParameterSigma(unsigned int i) const {assert(i < m_nPar); return m_parameterSigma[i];}
  unsigned int GetNumberOfParameters()           const {return m_nPar;}
  float        GetAngle(unsigned int i)          const {assert(i < m_nAngles); return m_angles[i];}

  bool         WriteToFile(QFile* file);

  unsigned int ShiftParameterIndexForItem(unsigned int hybrid, unsigned int sipm);
  unsigned int RotationParameterIndexForItem(unsigned int uniqueLayer, unsigned int hybrid, unsigned int sipm);

private:
  void ConstructIndexMap();

private:
  
  unsigned int m_nPar;
  unsigned int m_nAngles;
  float*       m_angles;
  float*       m_parameters;
  float*       m_parameterSigma;

  std::map<ArrayPair,int> m_indexMap;

};

#endif /* Parameters_hh */

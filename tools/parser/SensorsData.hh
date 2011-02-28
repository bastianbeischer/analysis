#ifndef SensorsData_hh
#define SensorsData_hh

#include <ctime>

class TFile;
class TTree;

class SensorsData
{
public:
  enum DataType { SENSORS, ATC, EBASS };
  
public:
  explicit SensorsData(DataType type, const char* file);
  ~SensorsData();

  int entryForTime(unsigned int time) const;
  float previousValue(const char* id, unsigned int time, int& diff);
  float nextValue(const char* id, unsigned int time, int& diff);
  float averageValue(const char* id, unsigned int time); // Will return NaN if out of bounds.
  float* values(unsigned int time) const;

  int numberOfKeys() const {return m_nKeys;}
  const char** keys() const {return m_keys;}
  TTree* tree() {return m_tree;}
  bool good() const {return m_good;}
  unsigned int firstTime() const {return m_firstTime;}
  
public:
  bool addRootFile(const char* file);

private:
  void init();

private:
  DataType m_type;

  TFile* m_file;
  TTree* m_tree;

  int m_nKeys;
  const char** m_keys;
  unsigned int m_firstTime;
  bool m_good;
  
  float        m_var;
  float*       m_values;

};

#endif /* SensorsData_hh */

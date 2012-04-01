#ifndef CalibrationCollection_hh
#define CalibrationCollection_hh

#include <TObject.h>
#include <TH1I.h>

#include <map>

class CalibrationCollection : public TObject
{
  
public:
  CalibrationCollection();
  CalibrationCollection(const CalibrationCollection&);
  ~CalibrationCollection();

  const TH1I* calibration(unsigned short) const;

  void addHistogram(unsigned short);
  void addValue(unsigned short, int);

private:
  std::map<unsigned short, TH1I*> m_calibrationHistograms;
  
  ClassDef( CalibrationCollection, 1 );

};

#endif /* CalibrationCollection_hh */

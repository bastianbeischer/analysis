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

  const TH1I* pedestalHistogram(unsigned short) const;
  void addPedestalHistogram(unsigned short);
  void addPedestalValue(unsigned short, int);

  const TH1I* ledHistogram(unsigned short) const;
  void addLedHistogram(unsigned short);
  void addLedValue(unsigned short, int);
private:
  std::map<unsigned short, TH1I*> m_pedestalHistograms;
  std::map<unsigned short, TH1I*> m_ledHistograms;
  
  ClassDef( CalibrationCollection, 1 );

};

#endif /* CalibrationCollection_hh */

#ifndef CalibrationCollection_hh
#define CalibrationCollection_hh

#include <TObject.h>
#include <TH1.h>

#include <map>

class CalibrationCollection : public TObject
{
  
public:
  CalibrationCollection();
  CalibrationCollection(const CalibrationCollection&);
  ~CalibrationCollection();

  const TH1S* pedestalHistogram(unsigned short) const;
  void addPedestalHistogram(unsigned short);
  void addPedestalValue(unsigned short, int);
  std::vector<unsigned short> pedestalIds() const;

  const TH1S* ledHistogram(unsigned short) const;
  void addLedHistogram(unsigned short);
  void addLedValue(unsigned short, int);
  std::vector<unsigned short> ledIds() const;
private:
  std::map<unsigned short, TH1S*> m_pedestalHistograms;
  std::map<unsigned short, TH1S*> m_ledHistograms;
  
  ClassDef( CalibrationCollection, 1 );

};

#endif /* CalibrationCollection_hh */

#ifndef ECALSignalHeight_hh
#define ECALSignalHeight_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QMap>

class TH2D;

class ECALSignalHeight : public AnalysisPlot, public H2DPlot {
public:
  explicit ECALSignalHeight();
  virtual ~ECALSignalHeight();
  virtual void processEvent(const AnalyzedEvent*);
private:
  QMap<unsigned short, int> m_bins;
};

#endif

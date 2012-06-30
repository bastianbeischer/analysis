#ifndef ECALSignalHeight_hh
#define ECALSignalHeight_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include <QMap>

class TH2D;

class ECALSignalHeight : public AnalysisPlot, public H2DProjectionPlot {
public:
  explicit ECALSignalHeight();
  virtual ~ECALSignalHeight();
  virtual void processEvent(const AnalyzedEvent*);
private:
  QMap<unsigned short, int> m_bins;
};

#endif

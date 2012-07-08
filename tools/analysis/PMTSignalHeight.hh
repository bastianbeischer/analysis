#ifndef PMTSignalHeight_hh
#define PMTSignalHeight_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class PMTSignalHeight : public AnalysisPlot, public H1DPlot {
public:
  explicit PMTSignalHeight(int id);
  virtual ~PMTSignalHeight();
  virtual void processEvent(const AnalyzedEvent*);
private:
  int m_id;
};

#endif

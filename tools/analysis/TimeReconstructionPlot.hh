#ifndef TimeReconstructionPlot_hh
#define TimeReconstructionPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class TimeReconstructionPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  enum Method {Mean, Median};
  TimeReconstructionPlot(Method);
  virtual ~TimeReconstructionPlot();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void finalize();
  virtual void draw(TCanvas*);
private:
  Method m_method;
};

#endif

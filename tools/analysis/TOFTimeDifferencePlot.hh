#ifndef TOFTimeDifferencePlot_hh
#define TOFTimeDifferencePlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TGraphErrors;
class TH2;

class TOFTimeDifferencePlot : public AnalysisPlot, public H2DPlot {
public:
  explicit TOFTimeDifferencePlot(unsigned short id);
  virtual ~TOFTimeDifferencePlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
  virtual void update();
private:
  unsigned short m_id;
  unsigned short m_updateCounter;
  TH2* m_normalizationHistogram;
};

#endif

#ifndef TOFTimeShiftPlot_hh
#define TOFTimeShiftPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TOFTimeShiftPlot : public AnalysisPlot, public H1DPlot {
public:
  TOFTimeShiftPlot(unsigned short topBarId, unsigned short bottomBarId, bool negativeSide);
  virtual ~TOFTimeShiftPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void update();
private:
  unsigned int m_topBarId;
  unsigned int m_bottomBarId;
  bool m_negativeSide;
};

#endif

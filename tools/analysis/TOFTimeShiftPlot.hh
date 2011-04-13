#ifndef TOFTimeShiftPlot_hh
#define TOFTimeShiftPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TOFTimeShiftPlot : public AnalysisPlot, public H2DPlot {
public:
  TOFTimeShiftPlot(unsigned short topBarId, unsigned short bottomBarId);
  virtual ~TOFTimeShiftPlot();
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
private:
  unsigned int m_topBarId;
  unsigned int m_bottomBarId;
};

#endif

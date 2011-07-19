#ifndef TOFTimeShiftPlot_hh
#define TOFTimeShiftPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class TOFTimeShiftPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  TOFTimeShiftPlot(unsigned short topBarId, unsigned short bottomBarId, unsigned short referenceChannel);
  virtual ~TOFTimeShiftPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  unsigned int m_topBarId;
  unsigned int m_bottomBarId;
  unsigned int m_referenceChannel;
};

#endif

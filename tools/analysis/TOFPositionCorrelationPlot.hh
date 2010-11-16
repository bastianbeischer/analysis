#ifndef TOFPositionCorrelationPlot_hh
#define TOFPositionCorrelationPlot_hh

#include "H2DPlot.hh"

class TOFPositionCorrelationPlot : public H2DPlot {
  public:
    explicit TOFPositionCorrelationPlot(unsigned short id);
    virtual ~TOFPositionCorrelationPlot();
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
    virtual void finalize();
  private:
    unsigned short m_id;
};

#endif

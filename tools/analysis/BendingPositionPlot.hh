#ifndef BendingPositionPlot_hh
#define BendingPositionPlot_hh

#include "H1DPlot.hh"

class BendingPositionPlot : public H1DPlot {
  public:
    BendingPositionPlot();
    virtual ~BendingPositionPlot();
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
    virtual void finalize();
};

#endif

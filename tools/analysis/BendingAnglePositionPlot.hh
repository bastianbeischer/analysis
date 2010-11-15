#ifndef BendingAnglePositionPlot_hh
#define BendingAnglePositionPlot_hh

#include "H2DPlot.hh"

class TCanvas;

class BendingAnglePositionPlot : public H2DPlot {
  public:
    explicit BendingAnglePositionPlot(double cut);
    virtual ~BendingAnglePositionPlot();
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
    virtual void finalize();
    virtual void draw(TCanvas* canvas);
  private:
    double m_cut;
    TH2D*  m_normHisto;

};

#endif

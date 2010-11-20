#ifndef H2DPlot_hh
#define H2DPlot_hh

#include "RootPlot.hh"

#include <QVector>

class TCanvas;
class TH2D;

class H2DPlot : virtual public RootPlot {
  public:
    H2DPlot();
    virtual ~H2DPlot();
    virtual void draw(TCanvas*);
    virtual void clear();
    TH2D* histogram();
    void setHistogram(TH2D*);
  private:
    TH2D* m_histogram;
};

#endif

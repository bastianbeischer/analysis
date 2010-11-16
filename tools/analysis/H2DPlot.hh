#ifndef H2DPlot_hh
#define H2DPlot_hh

#include "AnalysisPlot.hh"

class Hit;
class Track;
class SimpleEvent;

class TH2D;
class THStack;

class H2DPlot : public AnalysisPlot {
  public:
    H2DPlot(Topic);
    virtual ~H2DPlot();
    virtual void draw(TCanvas*);
    virtual void clear();
    TH2D* histogram();
    void setHistogram(TH2D*);
  private:
    TH2D* m_histogram;
};

#endif

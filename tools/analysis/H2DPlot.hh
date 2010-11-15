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
    virtual void draw(TCanvas*) const;
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0) = 0;
    virtual void finalize() = 0;
    virtual void clear();
    TH2D* histogram();
    void setHistogram(TH2D*);
  private:
    TH2D* m_histogram;
};

#endif

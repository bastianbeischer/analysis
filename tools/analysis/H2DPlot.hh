#ifndef H2DPlot_hh
#define H2DPlot_hh

#include "AnalysisPlot.hh"

class Hit;
class Track;
class SimpleEvent;

class TH2;
class TCanvas;

class H2DPlot : public AnalysisPlot {
  public:
    H2DPlot(Topic, const QString& title, int nBinsX, double xMin, double xMax, int nBinsY, double yMin, double yMax); 
    virtual ~H2DPlot();
    virtual void draw(TCanvas*) const;
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0) = 0;
    virtual void finalize() = 0;
    virtual void clear();
  protected:
    TH2* m_histogram;
};

#endif

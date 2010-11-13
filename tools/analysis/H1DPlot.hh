#ifndef H1DPlot_hh
#define H1DPlot_hh

#include "AnalysisPlot.hh"

class Hit;
class Track;
class SimpleEvent;

class TH1;
class TCanvas;

class H1DPlot : public AnalysisPlot {
  public:
    H1DPlot(Topic, const QString& title, int nBins, double xMin, double xMax);
    virtual ~H1DPlot();
    virtual void draw(TCanvas*) const;
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0) = 0;
    virtual void finalize() = 0;
    virtual void clear();
  protected:
    TH1* m_histogram;
};

#endif

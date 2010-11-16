#ifndef H1DPlot_hh
#define H1DPlot_hh

#include "AnalysisPlot.hh"

class Hit;
class Track;
class SimpleEvent;

class TH1D;

class H1DPlot : public AnalysisPlot {
  public:
    H1DPlot(Topic);
    virtual ~H1DPlot();
    virtual void draw(TCanvas*);
    virtual void clear();
    virtual void clear(int);
    int numberOfHistograms();
    TH1D* histogram(int i);
    void addHistogram(TH1D*);
  private:
    QVector<TH1D*> m_histograms;
};

#endif

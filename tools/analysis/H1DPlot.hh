#ifndef H1DPlot_hh
#define H1DPlot_hh

#include "AnalysisPlot.hh"

class Hit;
class Track;
class SimpleEvent;

class TH1D;
class THStack;

class H1DPlot : public AnalysisPlot {
  public:
    H1DPlot(Topic);
    virtual ~H1DPlot();
    virtual void draw(TCanvas*) const;
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0) = 0;
    virtual void finalize() = 0;
    virtual void clear();
    virtual void clear(int);
    int numberOfHistograms();
    TH1D* histogram(int i);
    void addHistogram(TH1D*);
    void setAxisTitles(const QString&, const QString&);
  private:
    THStack* m_histograms;
    QString m_xAxisTitle;
    QString m_yAxisTitle;
};

#endif

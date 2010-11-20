#ifndef TOFPositionCorrelationPlot_hh
#define TOFPositionCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TGraphErrors;
class TF1;

class TOFPositionCorrelationPlot : public AnalysisPlot, public H2DPlot {
  public:
    explicit TOFPositionCorrelationPlot(unsigned short id);
    virtual ~TOFPositionCorrelationPlot();
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, TrackSelection* = 0, SimpleEvent* = 0);
    virtual void finalize();
    virtual void draw(TCanvas*);
  private:
    unsigned short m_id;
    TGraphErrors* m_correlationGraph;
    TF1* m_correlationFunction;
};

#endif

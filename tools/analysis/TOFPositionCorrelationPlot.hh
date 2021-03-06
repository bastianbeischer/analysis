#ifndef TOFPositionCorrelationPlot_hh
#define TOFPositionCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TGraphErrors;

class TOFPositionCorrelationPlot : public AnalysisPlot, public H2DPlot {
public:
  explicit TOFPositionCorrelationPlot(unsigned short id);
  virtual ~TOFPositionCorrelationPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void finalize();
  virtual void draw(TCanvas*);
private:
  unsigned short m_id;
  TGraphErrors* m_correlationGraph;
};

#endif

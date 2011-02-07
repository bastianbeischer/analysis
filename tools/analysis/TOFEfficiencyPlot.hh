#ifndef TOFEfficiencyPlot_hh
#define TOFEfficiencyPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TGraphErrors;
class TH2;

class TOFEfficiencyPlot : public AnalysisPlot, public H2DPlot {
public:
  explicit TOFEfficiencyPlot(unsigned short id);
  virtual ~TOFEfficiencyPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
  virtual void draw(TCanvas*);
private:
  unsigned short m_id;
  TH2* m_normalizationHistogram;
};

#endif

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
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void finalize();
  virtual void update();
private:
  unsigned short m_id;
  unsigned short m_updateCounter;
  TH2* m_normalizationHistogram;
};

#endif

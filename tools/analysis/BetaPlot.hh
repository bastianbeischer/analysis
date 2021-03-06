#ifndef BetaPlot_hh
#define BetaPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TLine;

class BetaPlot : public AnalysisPlot, public H1DPlot {
public:
  BetaPlot();
  virtual ~BetaPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void update();
  virtual void draw(TCanvas*);
private:
  double m_cut;
  TLine* m_line;
};

#endif

#ifndef BendingAnglePositionPlot_hh
#define BendingAnglePositionPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TCanvas;

class BendingAnglePositionPlot : public AnalysisPlot, public H2DPlot {
public:
  BendingAnglePositionPlot(double cut);
  virtual ~BendingAnglePositionPlot();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void finalize();
private:
  double m_cut;
  TH2D*  m_normHisto;
};

#endif

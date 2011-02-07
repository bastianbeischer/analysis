#ifndef BarEfficiencyPlot_hh
#define BarEfficiencyPlot_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class TH2D;

class BarEfficiencyPlot : public PostAnalysisPlot, public H2DPlot {
public:
  BarEfficiencyPlot(TCanvas*, TCanvas*, TCanvas*, TCanvas*);
  virtual ~BarEfficiencyPlot();
private:
  TH2D* findHistogram(TCanvas*);
  double sumEntries(TH2D*);
};

#endif

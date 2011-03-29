#ifndef BarEfficiencyPlot_hh
#define BarEfficiencyPlot_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class TH2D;
class PostAnalysisCanvas;

class BarEfficiencyPlot : public PostAnalysisPlot, public H2DPlot {
public:
//<<<<<<< HEAD
//  BarEfficiencyPlot(TCanvas*, TCanvas*, TCanvas*, TCanvas*);
  BarEfficiencyPlot(QVector<TCanvas*> canvases);
  BarEfficiencyPlot(PostAnalysisCanvas*, PostAnalysisCanvas*, PostAnalysisCanvas*, PostAnalysisCanvas*);

  virtual ~BarEfficiencyPlot();
private:
  double sumEntries(TH2D*);
};

#endif

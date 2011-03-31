#ifndef MeanBarEfficiencyPlot_hh
#define MeanBarEfficiencyPlot_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class TH2D;
class BarEfficiencyPlot;

class MeanBarEfficiencyPlot : public PostAnalysisPlot, public H2DPlot {
public:
  MeanBarEfficiencyPlot(const QVector<BarEfficiencyPlot*>&);
  virtual ~MeanBarEfficiencyPlot();
private:
  double sumEntries(TH2D*);
};

#endif

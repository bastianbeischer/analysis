#ifndef MomentumSpectrumPlot_hh
#define MomentumSpectrumPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

class MomentumSpectrumPlot : public AnalysisPlot, public H1DPlot {
public:
  MomentumSpectrumPlot(Enums::ChargeSigns, bool inverted = false);
  ~MomentumSpectrumPlot();
  void processEvent(const AnalyzedEvent*);
  void update();
private:
  Enums::ChargeSigns m_type;
  bool m_inverted;
};

#endif /* MomentumSpectrumPlot_hh */

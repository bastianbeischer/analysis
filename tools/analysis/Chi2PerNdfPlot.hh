#ifndef Chi2PerNdfPlot_hh
#define Chi2PerNdfPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class Chi2PerNdfPlot : public AnalysisPlot, public H1DPlot
{
  
public:
  Chi2PerNdfPlot();
  ~Chi2PerNdfPlot();
  
  void processEvent(const AnalyzedEvent*);
  void update();

};

#endif /* Chi2PerNdfPlot_hh */

#ifndef Chi2Plot_hh
#define Chi2Plot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class Chi2Plot : public AnalysisPlot, public H1DPlot
{

public:
  Chi2Plot(unsigned short);
  ~Chi2Plot();

  void processEvent(const AnalyzedEvent*);
  void finalize();
  void update();

private:
  unsigned short m_ndf;

};

#endif /* Chi2Plot_hh */

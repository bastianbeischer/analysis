#ifndef Chi2Plot_hh
#define Chi2Plot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class Chi2Plot : public AnalysisPlot, public H1DPlot {
  
public:
  Chi2Plot();
  ~Chi2Plot();
  
  void processEvent(const QVector<Hit*>&, Track* = 0, TrackSelection* = 0, SimpleEvent* = 0);
  void finalize();

private:
  
  
};

#endif /* Chi2Plot_hh */

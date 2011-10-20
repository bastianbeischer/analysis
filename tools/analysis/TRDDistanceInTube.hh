#ifndef TRDDISTANCEINTUBE_HH
#define TRDDISTANCEINTUBE_HH

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TRDDistanceInTube : public AnalysisPlot, public H1DPlot
{

public:
  TRDDistanceInTube(Enums::AnalysisTopic);
  ~TRDDistanceInTube();

  virtual void processEvent(const AnalyzedEvent*);

};

#endif // TRDDISTANCEINTUBE_HH

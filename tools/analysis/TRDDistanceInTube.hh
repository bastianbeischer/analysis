#ifndef TRDDISTANCEINTUBE_HH
#define TRDDISTANCEINTUBE_HH

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TRDDistanceInTube : public AnalysisPlot, public H1DPlot
{

public:
  TRDDistanceInTube(AnalysisPlot::Topic);
  ~TRDDistanceInTube();

  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();

};

#endif // TRDDISTANCEINTUBE_HH

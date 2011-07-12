#ifndef AnalysisPlot_hh
#define AnalysisPlot_hh

#include "RootPlot.hh"
#include "EventDestination.hh"

class AnalysisPlot : virtual public RootPlot, public EventDestination {
public:
  enum Topic {TopicBegin, SignalHeightTracker = TopicBegin, SignalHeightTRD,
              ClusterShapeTracker, ClusterShapeTRD, TimeOverThreshold, Tracking, Occupancy,
              ResidualsTracker, ResidualsTRD, MomentumReconstruction, EfficiencyTOF,
              ResolutionTOF, CalibrationTOF, MiscellaneousTracker, MiscellaneousTRD,
              MiscellaneousTOF, SlowControl, MonteCarloTracker, MonteCarloTRD, MonteCarloTOF,
              MonteCarlo, Testbeam, TopicEnd};
public:
  AnalysisPlot(Topic);
  virtual ~AnalysisPlot();
  Topic topic() const;
private:
  Topic m_topic;
};

#endif

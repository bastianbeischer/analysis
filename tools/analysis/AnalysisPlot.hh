#ifndef AnalysisPlot_hh
#define AnalysisPlot_hh

#include "RootPlot.hh"
#include "EventDestination.hh"

#include <QMutex>

class Hit;
class Particle;
class SimpleEvent;
class QWidget;

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
  QWidget* secondaryWidget() const {return m_secondaryWidget;}
protected:
  QMutex m_mutex;
  QWidget* m_secondaryWidget;
private:
  Topic m_topic;
};

#endif

#ifndef AnalysisPlot_hh
#define AnalysisPlot_hh

#include "RootPlot.hh"

#include <QMutex>

class Hit;
class Track;
class SimpleEvent;

class AnalysisPlot : virtual public RootPlot {
public:
  enum Topic {TopicBegin, SignalHeightTracker = TopicBegin, SignalHeightTRD,
              ClusterShapeTracker, ClusterShapeTRD, TimeOverThreshold, Tracking, Occupancy,
              ResidualsTracker, ResidualsTRD, MomentumReconstruction,
              EfficiencyTOF, ResolutionTOF, MiscellaneousTracker, MiscellaneousTRD,
              MiscellaneousTOF, TopicEnd};
public:
  AnalysisPlot(Topic);
  virtual ~AnalysisPlot();
  Topic topic() const;
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0) = 0;
  virtual void finalize() {}
  virtual void update() {}
protected:
  QMutex m_mutex;
private:
  Topic m_topic;
};

#endif

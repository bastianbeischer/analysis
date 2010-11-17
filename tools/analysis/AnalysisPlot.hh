#ifndef AnalysisPlot_hh
#define AnalysisPlot_hh

#include "RootPlot.hh"

#include <QMutex>

class Hit;
class Track;
class SimpleEvent;

class AnalysisPlot : virtual public RootPlot {
  public:
    enum Topic {TopicBegin, SignalHeightUpperTracker = TopicBegin,
                SignalHeightLowerTracker, SignalHeightTRD, ClusterLengthUpperTracker,
                ClusterLengthLowerTracker, ClusterLengthTRD, TimeOverThreshold, Tracking, Occupancy,
                ResidualsUpperTracker, ResidualsLowerTracker, ResidualsTRD, MomentumReconstruction,
                MiscellaneousTracker, MiscellaneousTRD, MiscellaneousTOF, TopicEnd};
  public:
    AnalysisPlot(Topic);
    virtual ~AnalysisPlot();
    Topic topic() const;
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0) = 0;
    virtual void finalize() = 0;
  protected:
    QMutex m_mutex;
  private:
    Topic m_topic;
};

#endif

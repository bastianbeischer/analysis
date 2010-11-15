#ifndef AnalysisPlot_hh
#define AnalysisPlot_hh

#include <QVector>
#include <QString>

class Hit;
class Track;
class SimpleEvent;
class TCanvas;

class AnalysisPlot {
  public:
    enum Topic {TopicBegin, SignalHeightUpperTracker = TopicBegin,
      SignalHeightLowerTracker, SignalHeightTRD, TimeOverThreshold, Tracking, Occupancy,
      ResidualsUpperTracker, ResidualsLowerTracker, ResidualsTRD, MomentumReconstruction,
      MiscellaneousTracker, MiscellaneousTRD, MiscellaneousTOF, TopicEnd};
  public:
    AnalysisPlot(Topic);
    virtual ~AnalysisPlot();
    unsigned long id();
    void setTitle(const QString& title);
    const QString& title() const;
    Topic topic() const;
    virtual void draw(TCanvas*) = 0;
    virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0) = 0;
    virtual void finalize() = 0;
    virtual void clear() = 0;
  private:
    Topic m_topic;
    QString m_title;
    unsigned long m_id;
    static unsigned long s_analysisPlotCounter;
};

#endif

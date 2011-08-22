#ifndef TimeBetweenEventsPlot_hh
#define TimeBetweenEventsPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TimeBetweenEventsPlot : public AnalysisPlot, public H1DPlot {
public:
  TimeBetweenEventsPlot(int numberOfThreads);
  virtual ~TimeBetweenEventsPlot();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  const int m_numberOfThreads;
  double m_lastEventTime;
};

#endif

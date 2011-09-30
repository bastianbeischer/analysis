#ifndef AnalysisPlot_hh
#define AnalysisPlot_hh

#include "RootPlot.hh"
#include "EventDestination.hh"
#include "AnalysisTopic.hh"

class AnalysisPlot : virtual public RootPlot, public EventDestination {
public:
  AnalysisPlot(AnalysisTopic);
  virtual ~AnalysisPlot();
  virtual void finalize() {}
  virtual void update() {}
  AnalysisTopic topic() const;
private:
  AnalysisTopic m_topic;
};

#endif

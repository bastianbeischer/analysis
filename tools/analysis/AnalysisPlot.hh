#ifndef AnalysisPlot_hh
#define AnalysisPlot_hh

#include "RootPlot.hh"
#include "EventDestination.hh"
#include "AnalysisTopic.hh"
#include "Enums.hh"

class AnalysisPlot : virtual public RootPlot, public EventDestination {
public:
  AnalysisPlot(Enums::AnalysisTopic);
  virtual ~AnalysisPlot();
  virtual void finalize() {}
  virtual void update() {}
  virtual bool isPlotCollection();
  Enums::AnalysisTopic topic() const;
private:
  Enums::AnalysisTopic m_topic;
};

#endif

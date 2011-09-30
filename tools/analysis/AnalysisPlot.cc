#include "AnalysisPlot.hh"

#include <QWidget>

AnalysisPlot::AnalysisPlot(AnalysisTopic topic)
  : m_topic(topic)
{
  setNeedsLocking(true);
}

AnalysisPlot::~AnalysisPlot()
{
}

AnalysisTopic AnalysisPlot::topic() const
{
  return m_topic;
}

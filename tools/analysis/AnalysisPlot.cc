#include "AnalysisPlot.hh"

#include <QWidget>

AnalysisPlot::AnalysisPlot(Topic topic)
  : m_topic(topic)
{
  setNeedsLocking(true);
}

AnalysisPlot::~AnalysisPlot()
{
}

AnalysisPlot::Topic AnalysisPlot::topic() const
{
  return m_topic;
}

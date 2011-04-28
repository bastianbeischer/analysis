#include "AnalysisPlot.hh"

AnalysisPlot::AnalysisPlot(Topic topic)
  : m_secondaryWidget(0)
  , m_topic(topic)
{
}

AnalysisPlot::~AnalysisPlot()
{}

AnalysisPlot::Topic AnalysisPlot::topic() const
{
  return m_topic;
}

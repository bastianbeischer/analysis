#include "AnalysisPlot.hh"

unsigned long AnalysisPlot::s_analysisPlotCounter = 0;

AnalysisPlot::AnalysisPlot(Topic topic)
  : m_topic(topic)
  , m_title("")
  , m_id(s_analysisPlotCounter)
{
  ++s_analysisPlotCounter;
}

AnalysisPlot::~AnalysisPlot()
{}

unsigned long AnalysisPlot::id()
{
  return m_id;
}

void AnalysisPlot::setTitle(const QString& title)
{
  m_title = title;
}

const QString& AnalysisPlot::title() const
{
  return m_title;
}
    
AnalysisPlot::Topic AnalysisPlot::topic() const
{
  return m_topic;
}

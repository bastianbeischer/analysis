#include "AnalysisPlot.hh"

#include <QWidget>

AnalysisPlot::AnalysisPlot(Enums::AnalysisTopic topic)
  : m_topic(topic)
{
  setNeedsLocking(true);
}

AnalysisPlot::~AnalysisPlot()
{
}

Enums::AnalysisTopic AnalysisPlot::topic() const
{
  return m_topic;
}

#include "AnalysisPlot.hh"

#include <QWidget>

AnalysisPlot::AnalysisPlot(Topic topic)
  : m_secondaryWidget(0)
  , m_topic(topic)
{
}

AnalysisPlot::~AnalysisPlot()
{
  delete m_secondaryWidget;
}

AnalysisPlot::Topic AnalysisPlot::topic() const
{
  return m_topic;
}

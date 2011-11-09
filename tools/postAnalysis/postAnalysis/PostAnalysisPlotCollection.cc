#include "PostAnalysisPlotCollection.hh"

#include "RootQtWidget.hh"

#include <QLayout>
#include <QDebug>

PostAnalysisPlotCollection::PostAnalysisPlotCollection()
: QObject()
, m_selectedPlot(0)
{
  QWidget* widget = new QWidget;
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  widget->setLayout(layout);
  setSecondaryWidget(widget);
}

PostAnalysisPlotCollection::~PostAnalysisPlotCollection()
{
  qDeleteAll(m_plots);
}

void PostAnalysisPlotCollection::addPlot(PostAnalysisPlot* plot) {
  m_plots.append(plot);
}

void PostAnalysisPlotCollection::unzoom()
{
  if (m_selectedPlot >= 0 && m_selectedPlot < m_plots.size())
    m_plots[m_selectedPlot]->unzoom();
}

void PostAnalysisPlotCollection::positionChanged(double x, double y)
{
  if (m_selectedPlot >= 0 && m_selectedPlot < m_plots.size())
    m_plots[m_selectedPlot]->positionChanged(x,y);
}

void PostAnalysisPlotCollection::draw(TCanvas* can)
{
  if (m_selectedPlot >= 0 && m_selectedPlot < m_plots.size())
    m_plots[m_selectedPlot]->draw(can);

  QWidget* collectionSecWidget = secondaryWidget();
  if (collectionSecWidget != 0) {
    QLayout* layout = collectionSecWidget->layout();
    if (layout->count() > 1) {
      QWidget* prevWidget = layout->itemAt(1)->widget();
      layout->removeWidget(prevWidget);
      prevWidget->close();
    }

    QWidget* plotSecWidget = m_plots[m_selectedPlot]->secondaryWidget();
    if (plotSecWidget != 0) {
      layout->addWidget(plotSecWidget);
      plotSecWidget->show();
    }
  }

  RootPlot::draw(can);
  gPad->Update();
}

void PostAnalysisPlotCollection::selectPlot(int plot)
{
  m_selectedPlot = plot;
  draw(gPad->GetCanvas());
}

bool PostAnalysisPlotCollection::isPostAnalysisPlotCollection()
{
  return true;
}

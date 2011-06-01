#include "PlotCollection.hh"

#include "Plotter.hh"

#include "RootQtWidget.hh"

#include <QLayout>

PlotCollection::PlotCollection(AnalysisPlot::Topic topic) :
  QObject(),
  AnalysisPlot(topic),
  m_selectedPlot(0)
{
  QWidget* widget = new QWidget;
  widget->setLayout(new QVBoxLayout);
  setSecondaryWidget(widget);
}

PlotCollection::~PlotCollection()
{
  qDeleteAll(m_plots);
}

void PlotCollection::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  foreach(AnalysisPlot* plot, m_plots)
    plot->processEvent(hits, particle, event);
}

void PlotCollection::finalize()
{
  foreach(AnalysisPlot* plot, m_plots)
    plot->finalize();
}

void PlotCollection::update()
{
  foreach(AnalysisPlot* plot, m_plots)
    plot->update();
}

void PlotCollection::unzoom()
{
  if (m_selectedPlot >= 0 && m_selectedPlot < m_plots.size())
    m_plots[m_selectedPlot]->unzoom();
}

void PlotCollection::positionChanged(double x, double y)
{
  if (m_selectedPlot >= 0 && m_selectedPlot < m_plots.size())
    m_plots[m_selectedPlot]->positionChanged(x,y);
}

void PlotCollection::draw(TCanvas* can)
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
  Plotter::rootWidget()->updateCanvas();
}

void PlotCollection::saveForPostAnalysis(TCanvas* can)
{
  int prevSelectedPlot = m_selectedPlot;
  for (m_selectedPlot = 0; m_selectedPlot < m_plots.size(); m_selectedPlot++) {
    draw(can);
    can->SetName(qPrintable(m_plots.at(m_selectedPlot)->title() + " canvas"));
    can->Write();
  }
  selectPlot(prevSelectedPlot);
}

void PlotCollection::selectPlot(int plot)
{
  m_selectedPlot = plot;
  draw(Plotter::rootWidget()->GetCanvas());
}

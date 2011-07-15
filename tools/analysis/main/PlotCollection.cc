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
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  widget->setLayout(layout);
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

void PlotCollection::saveForPostAnalysis()
{
  for (int i = 0; i < m_plots.size(); ++i) {
    TCanvas* canvas = Plotter::rootWidget()->GetCanvas();
    m_plots[i]->draw(canvas);
    canvas->SetName(qPrintable(m_plots[i]->title() + " canvas"));
    canvas->Write();
  }
}

void PlotCollection::selectPlot(int plot)
{
  m_selectedPlot = plot;
  draw(Plotter::rootWidget()->GetCanvas());
}

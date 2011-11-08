#include "PostAnalysisPlotCollection.hh"

#include "RootQtWidget.hh"
#include "PostAnalysisCanvas.hh"

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

void PostAnalysisPlotCollection::addPlot(PostAnalysisPlot* plot, PostAnalysisCanvas* canvas) {
  m_plots.append(plot);
  m_canvas.append(canvas);
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

void PostAnalysisPlotCollection::saveForPostAnalysis(TCanvas* canvas)
{
  canvas->cd();
  for (int i = 0; i < m_plots.size(); ++i) {
    m_plots[i]->draw(canvas);
    canvas->SetName(qPrintable(m_plots[i]->title() + " canvas"));
    canvas->Write();
  }
}

void PostAnalysisPlotCollection::selectPlot(int plot)
{
  m_selectedPlot = plot;
  draw(m_canvas.at(plot)->canvas());
}

bool PostAnalysisPlotCollection::isPostAnalysisPlotCollection()
{
  return true;
}

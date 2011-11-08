#include "SignalHeightTemperatureCorrelationPlotCollection.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "SignalHeightTemperatureCorrelationPlot.hh"
#include "TrackerSipmSelectionWidget.hh"

#include <QLayout>

SignalHeightTemperatureCorrelationPlotCollection::SignalHeightTemperatureCorrelationPlotCollection()
  : PlotCollection(Enums::SignalHeightTracker)
{
  QStringList moduleIDs;
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tracker) {
      moduleIDs.append(qPrintable(QString("0x%1").arg(element->id(), 0, 16)));
      addPlot(new SignalHeightTemperatureCorrelationPlot(element->id()));
    }
  }
  TrackerSipmSelectionWidget* widget = new TrackerSipmSelectionWidget(moduleIDs);
  secondaryWidget()->layout()->addWidget(widget);
  connect(widget, SIGNAL(sipmChanged(int)), this, SLOT(selectPlot(int)));
  setTitle("temperature correlation - all channels");
}

SignalHeightTemperatureCorrelationPlotCollection::~SignalHeightTemperatureCorrelationPlotCollection()
{
}


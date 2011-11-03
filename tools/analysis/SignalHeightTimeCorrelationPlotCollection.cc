#include "SignalHeightTimeCorrelationPlotCollection.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "SignalHeightTimeCorrelationPlot.hh"
#include "TrackerSipmSelectionWidget.hh"

#include <QLayout>

SignalHeightTimeCorrelationPlotCollection::SignalHeightTimeCorrelationPlotCollection(const QDateTime& first, const QDateTime& last) :
  PlotCollection(Enums::SignalHeightTracker)
{
  QStringList moduleIDs;
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tracker) {
      moduleIDs.append(qPrintable(QString("0x%1").arg(element->id(), 0, 16)));
      addPlot(new SignalHeightTimeCorrelationPlot(element->id(), first, last));
    }
  }
  TrackerSipmSelectionWidget* widget = new TrackerSipmSelectionWidget(moduleIDs);
  secondaryWidget()->layout()->addWidget(widget);
  connect(widget, SIGNAL(sipmChanged(int)), this, SLOT(selectPlot(int)));
  setTitle("time correlation - all channels");
}

SignalHeightTimeCorrelationPlotCollection::~SignalHeightTimeCorrelationPlotCollection()
{
}


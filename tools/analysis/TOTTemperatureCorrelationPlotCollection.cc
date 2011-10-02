#include "TOTTemperatureCorrelationPlotCollection.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "TOTTemperatureCorrelationPlot.hh"
#include "TOFChannelSelectionWidget.hh"

#include <QLayout>

TOTTemperatureCorrelationPlotCollection::TOTTemperatureCorrelationPlotCollection() :
  PlotCollection(Enums::TimeOverThreshold)
{
  QStringList moduleIDs;

  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tof){
      for(unsigned short tubeNo = 0; tubeNo < 4; tubeNo++) {
        moduleIDs.append("0x" + QString::number(element->id() | tubeNo, 16));
        addPlot(new TOTTemperatureCorrelationPlot(element->id() | tubeNo));
      }
    }
  }

  TOFChannelSelectionWidget* widget = new TOFChannelSelectionWidget(moduleIDs);
  secondaryWidget()->layout()->addWidget(widget);
  connect(widget, SIGNAL(channelChanged(int)), this, SLOT(selectPlot(int)));

  setTitle("temperature correlation - all channels");
}

TOTTemperatureCorrelationPlotCollection::~TOTTemperatureCorrelationPlotCollection()
{
}


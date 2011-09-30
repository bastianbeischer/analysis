#include "TOTTimeCorrelationPlotCollection.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "TOTTimeCorrelationPlot.hh"
#include "TOFChannelSelectionWidget.hh"

#include <QLayout>

TOTTimeCorrelationPlotCollection::TOTTimeCorrelationPlotCollection(const QDateTime& first, const QDateTime& last) :
  PlotCollection(AnalysisTopic::TimeOverThreshold)
{
  QStringList moduleIDs;

  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tof){
      for(unsigned short tubeNo = 0; tubeNo < 4; tubeNo++) {
        moduleIDs.append("0x" + QString::number(element->id() | tubeNo,16));
        addPlot(new TOTTimeCorrelationPlot(element->id() | tubeNo, first, last));
      }
    }
  }

  TOFChannelSelectionWidget* widget = new TOFChannelSelectionWidget(moduleIDs);
  secondaryWidget()->layout()->addWidget(widget);
  connect(widget, SIGNAL(channelChanged(int)), this, SLOT(selectPlot(int)));

  setTitle("time correlation - all channels");
}

TOTTimeCorrelationPlotCollection::~TOTTimeCorrelationPlotCollection()
{
}


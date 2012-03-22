#include "TOTTimeCorrelationPlotCollection.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "TOTTimeCorrelationPlot.hh"
#include "TOFChannelSelectionWidget.hh"

#include <QLayout>
#include <QDebug>

TOTTimeCorrelationPlotCollection::TOTTimeCorrelationPlotCollection(const QDateTime& first, const QDateTime& last)
  : PlotCollection(Enums::TimeOverThreshold)
{
  addRequiredEventFlags(Enums::TrackGood);

  QStringList moduleIDs;
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tof){
      for (unsigned short barNo = 0; barNo < 4; ++barNo) {
        moduleIDs.append("0x" + QString::number(element->id() | barNo, 16));
        addPlot(new TOTTimeCorrelationPlot(element->id() | barNo, first, last));
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


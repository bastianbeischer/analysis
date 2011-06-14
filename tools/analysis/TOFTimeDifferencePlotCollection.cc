#include "TOFTimeDifferencePlotCollection.hh"
#include "TOFTimeDifferencePlot.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "TOFChannelSelectionWidget.hh"

#include <QLayout>

TOFTimeDifferencePlotCollection::TOFTimeDifferencePlotCollection() :
  PlotCollection(AnalysisPlot::CalibrationTOF)
{
  QStringList moduleIDs;
  const ElementIterator itEnd = Setup::instance()->lastElement();
  for (ElementIterator it = Setup::instance()->firstElement(); it != itEnd; ++it) {
    DetectorElement* element = *it;
    if (element->type() == DetectorElement::tof) {
      addPlot(new TOFTimeDifferencePlot(element->id()));
      moduleIDs.append(QString("0x%1").arg(element->id(), 0, 16));
    }
  }
  TOFChannelSelectionWidget* widget = new TOFChannelSelectionWidget(moduleIDs);
  connect(widget, SIGNAL(channelChanged(int)), this, SLOT(selectPlot(int)));
  secondaryWidget()->layout()->addWidget(widget);
  setTitle("time difference collection");
}

TOFTimeDifferencePlotCollection::~TOFTimeDifferencePlotCollection()
{
}


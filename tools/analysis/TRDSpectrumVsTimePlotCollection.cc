#include "TRDSpectrumVsTimePlotCollection.hh"

#include "DetectorSelectionWidget.hh"
#include "TRDSpectrumVsTimePlot.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

#include <QLayout>

TRDSpectrumVsTimePlotCollection::TRDSpectrumVsTimePlotCollection(const QDateTime& first, const QDateTime& last) :
  PlotCollection(Enums::SignalHeightTRD)
{
  QStringList moduleIDs;

  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd){
      moduleIDs.append("0x" + QString::number(element->id(), 16));
      addPlot(new TRDSpectrumVsTimePlot(first, last, element->id(),TRDSpectrumPlot::module));
      //for(unsigned short tubeNo = 0; tubeNo < 16; tubeNo++) {
      //  addPlot(new TRDSpectrumVsTimePlot(element->id() | tubeNo,TRDSpectrumPlot::channel, first, last));
      //}
    }
  }

  DetectorSelectionWidget* selectionWidget = new DetectorSelectionWidget(moduleIDs, 0);
  secondaryWidget()->layout()->addWidget(selectionWidget);
  connect(selectionWidget, SIGNAL(selectPlot(int)), this, SLOT(selectPlot(int)));

  setTitle("spectrum vs time - all modules");
}

TRDSpectrumVsTimePlotCollection::~TRDSpectrumVsTimePlotCollection()
{
}

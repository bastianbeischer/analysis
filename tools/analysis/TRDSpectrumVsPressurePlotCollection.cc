#include "TRDSpectrumVsPressurePlotCollection.hh"

#include "DetectorSelectionWidget.hh"
#include "TRDSpectrumVsPressurePlot.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

#include <QLayout>

TRDSpectrumVsPressurePlotCollection::TRDSpectrumVsPressurePlotCollection() :
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
      addPlot(new TRDSpectrumVsPressurePlot(element->id(),TRDSpectrumPlot::module));
      for(unsigned short tubeNo = 0; tubeNo < 16; tubeNo++) {
        addPlot(new TRDSpectrumVsPressurePlot(element->id() | tubeNo,TRDSpectrumPlot::channel));
      }
    }
  }

  DetectorSelectionWidget* selectionWidget = new DetectorSelectionWidget(moduleIDs, 16);
  secondaryWidget()->layout()->addWidget(selectionWidget);
  connect(selectionWidget, SIGNAL(selectPlot(int)), this, SLOT(selectPlot(int)));

  setTitle("spectrum vs pressure - all modules");
}

TRDSpectrumVsPressurePlotCollection::~TRDSpectrumVsPressurePlotCollection()
{
}

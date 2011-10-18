#include "TRDSpectrumVsTemperaturePlotCollection.hh"

#include "DetectorSelectionWidget.hh"
#include "TRDSpectrumVsTemperaturePlot.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

#include <QLayout>

TRDSpectrumVsTemperaturePlotCollection::TRDSpectrumVsTemperaturePlotCollection() :
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
      addPlot(new TRDSpectrumVsTemperaturePlot(element->id(),TRDSpectrumPlot::module));
      for(unsigned short tubeNo = 0; tubeNo < 16; tubeNo++) {
        addPlot(new TRDSpectrumVsTemperaturePlot(element->id() | tubeNo,TRDSpectrumPlot::channel));
      }
    }
  }

  DetectorSelectionWidget* selectionWidget = new DetectorSelectionWidget(moduleIDs, 16);
  secondaryWidget()->layout()->addWidget(selectionWidget);
  connect(selectionWidget, SIGNAL(selectPlot(int)), this, SLOT(selectPlot(int)));

  setTitle("spectrum vs temperature - all modules");
}

TRDSpectrumVsTemperaturePlotCollection::~TRDSpectrumVsTemperaturePlotCollection()
{
}

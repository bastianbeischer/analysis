#include "TRDSpectrumPlotCollection.hh"

#include "DetectorSelectionWidget.hh"
#include "TRDSpectrumPlot.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

TRDSpectrumPlotCollection::TRDSpectrumPlotCollection() :
  PlotCollection(AnalysisPlot::SignalHeightTRD)
{
  QStringList moduleIDs;

  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd){
      moduleIDs.append("0x" + QString::number(element->id(), 16));
      addPlot(new TRDSpectrumPlot(element->id(),TRDSpectrumPlot::module));
      for(unsigned short tubeNo = 0; tubeNo < 16; tubeNo++) {
        addPlot(new TRDSpectrumPlot(element->id() | tubeNo,TRDSpectrumPlot::channel));
      }
    }
  }

  DetectorSelectionWidget* selectionWidget = new DetectorSelectionWidget(moduleIDs, 16);
  setSecondaryWidget(selectionWidget);
  connect(selectionWidget, SIGNAL(selectPlot(int)), this, SLOT(selectPlot(int)));

  setTitle("spectrum - all modules");
}

TRDSpectrumPlotCollection::~TRDSpectrumPlotCollection()
{
}

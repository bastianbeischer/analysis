#include "TRDCombinedPlot.hh"

#include "DetectorSelectionWidget.hh"
#include "TRDSpectrumPlot.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

TRDCombinedPlot::TRDCombinedPlot() :
  PlotCollection(AnalysisPlot::SignalHeightTRD)
{
  QVector<unsigned short> moduleIDs;

  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd){
      moduleIDs.append(element->id());
      addPlot(new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, element->id(),TRDSpectrumPlot::module));
      for(unsigned short tubeNo = 0; tubeNo < 16; tubeNo++) {
        addPlot(new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, element->id() | tubeNo,TRDSpectrumPlot::channel));
      }
    }
  }

  DetectorSelectionWidget* selectionWidget = new DetectorSelectionWidget(moduleIDs, 16);
  m_secondaryWidget = selectionWidget;
  connect(selectionWidget, SIGNAL(selectPlot(int)), this, SLOT(selectPlot(int)));

  setTitle("TRD spectrum plot collection");
}

TRDCombinedPlot::~TRDCombinedPlot()
{
}

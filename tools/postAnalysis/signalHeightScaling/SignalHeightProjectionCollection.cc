#include "SignalHeightProjectionCollection.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "SignalHeightProjection.hh"
#include "TrackerSipmSelectionWidget.hh"
#include "PostAnalysisCanvas.hh"

#include <QLayout>

SignalHeightProjectionCollection::SignalHeightProjectionCollection(PostAnalysisCanvas* canvas)
{
  QStringList moduleIDs;
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tracker) {
      moduleIDs.append(qPrintable(QString("0x%1").arg(element->id(), 0, 16)));
      addPlot(new SignalHeightProjection(canvas, element->id()));
    }
  }
  TrackerSipmSelectionWidget* widget = new TrackerSipmSelectionWidget(moduleIDs);
  secondaryWidget()->layout()->addWidget(widget);
  connect(widget, SIGNAL(sipmChanged(int)), this, SLOT(selectPlot(int)));
  setTitle("signal height");
}

SignalHeightProjectionCollection::~SignalHeightProjectionCollection()
{
}


#include "SignalHeightTimeCorCollection.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "SignalHeightTimeCor.hh"
#include "TrackerSipmSelectionWidget.hh"
#include "PostAnalysisCanvas.hh"

#include <TFile.h>

#include <QLayout>

SignalHeightTimeCorCollection::SignalHeightTimeCorCollection(TFile* file)
{
  QStringList moduleIDs;
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tracker) {
      moduleIDs.append(qPrintable(QString("0x%1").arg(element->id(), 0, 16)));
      QString plotName = QString("signal height time correlcation 0x%1 canvas").arg(element->id(), 0, 16);
      PostAnalysisCanvas* canvas = PostAnalysisCanvas::fromFile(file, plotName);
      addPlot(new SignalHeightTimeCor(canvas, element->id()), canvas);
    }
  }
  TrackerSipmSelectionWidget* widget = new TrackerSipmSelectionWidget(moduleIDs);
  secondaryWidget()->layout()->addWidget(widget);
  connect(widget, SIGNAL(sipmChanged(int)), this, SLOT(selectPlot(int)));
  setTitle("signal height time dependent");
}

SignalHeightTimeCorCollection::~SignalHeightTimeCorCollection()
{
}


#include "SignalHeightCorrelationCollection.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "SignalHeightTempCor.hh"
#include "SignalHeightTimeCor.hh"
#include "SignalHeightTimeCor2D.hh"
#include "TrackerSipmSelectionWidget.hh"
#include "PostAnalysisCanvas.hh"

#include <TFile.h>

#include <QLayout>

SignalHeightCorrelationCollection::SignalHeightCorrelationCollection(Type type, TFile* file)
{
  QStringList moduleIDs;
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tracker) {
      unsigned short id = element->id();
      moduleIDs.append(qPrintable(QString("0x%1").arg(id, 0, 16)));
      PostAnalysisPlot* plot = 0;
      if (type == Time) {
        setTitle("signal height time dependent");
        const QString& plotName = QString("signal height time correlation 0x%1 canvas").arg(id, 0, 16);
        plot = new SignalHeightTimeCor(PostAnalysisCanvas::fromFile(file, plotName), id);
      }
      if (type == Time2D) {
        setTitle("signal height 2d time dependent");
        const QString& plotName = QString("signal height time correlation 0x%1 canvas").arg(id, 0, 16);
        plot = new SignalHeightTimeCor2D(PostAnalysisCanvas::fromFile(file, plotName), id);
      }
      if (type == Temperature) {
        setTitle("signal height temp. dependent");
        const QString& plotName = QString("signal height temperature correlation 0x%1 canvas").arg(id, 0, 16);
        plot = new SignalHeightTempCor(PostAnalysisCanvas::fromFile(file, plotName), id);
      }
      addPlot(plot);
    }
  }
  TrackerSipmSelectionWidget* widget = new TrackerSipmSelectionWidget(moduleIDs);
  secondaryWidget()->layout()->addWidget(widget);
  connect(widget, SIGNAL(sipmChanged(int)), this, SLOT(selectPlot(int)));
}

SignalHeightCorrelationCollection::~SignalHeightCorrelationCollection()
{
}

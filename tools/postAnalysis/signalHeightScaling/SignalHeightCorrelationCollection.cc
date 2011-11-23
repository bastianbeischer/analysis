#include "SignalHeightCorrelationCollection.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "SignalHeightTempCorrelation.hh"
#include "SignalHeightTimeCorrelation.hh"
#include "SignalHeightTimeCorrelation2D.hh"
#include "TrackerSipmSelectionWidget.hh"
#include "PostAnalysisCanvas.hh"
#include "LocationSelectonWidget.hh"

#include <TFile.h>

#include <QLayout>
#include <QVBoxLayout>

SignalHeightCorrelationCollection::SignalHeightCorrelationCollection(Type type, TFile* file)
{
  QWidget* widget = new QWidget;
  QVBoxLayout* layout = new QVBoxLayout(widget);
  LocationSelectonWidget* locationSelector = 0;
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
        if (!locationSelector) {
          locationSelector = new LocationSelectonWidget;
          layout->addWidget(locationSelector);
        }
        setTitle("signal height time dependent");
        const QString& plotName = QString("signal height time correlation 0x%1 canvas").arg(id, 0, 16);
        plot = new SignalHeightTimeCorrelation(PostAnalysisCanvas::fromFile(file, plotName), id, locationSelector->situation());
        connect(locationSelector, SIGNAL(selectionChanged(Enums::Situation)), static_cast<SignalHeightTimeCorrelation*>(plot), SLOT(updateLocation(Enums::Situation)));
      }
      if (type == Time2D) {
        setTitle("signal height 2d time dependent");
        const QString& plotName = QString("signal height time correlation 0x%1 canvas").arg(id, 0, 16);
        plot = new SignalHeightTimeCorrelation2D(PostAnalysisCanvas::fromFile(file, plotName), id);
      }
      if (type == Temperature) {
        setTitle("signal height temp. dependent");
        const QString& plotName = QString("signal height temperature correlation 0x%1 canvas").arg(id, 0, 16);
        plot = new SignalHeightTempCorrelation(PostAnalysisCanvas::fromFile(file, plotName), id);
      }
      addPlot(plot);
    }
  }
  TrackerSipmSelectionWidget* sipmSelector = new TrackerSipmSelectionWidget(moduleIDs);
  layout->addWidget(sipmSelector);
  secondaryWidget()->layout()->addWidget(widget);
  connect(sipmSelector, SIGNAL(sipmChanged(int)), this, SLOT(selectPlot(int)));
}

SignalHeightCorrelationCollection::~SignalHeightCorrelationCollection()
{
}

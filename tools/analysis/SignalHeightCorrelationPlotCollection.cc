#include "SignalHeightCorrelationPlotCollection.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "SignalHeightCorrelationPlot.hh"
#include "TrackerSipmSelectionWidget.hh"

#include <QLayout>

#include <TH2D.h>

SignalHeightCorrelationPlotCollection::SignalHeightCorrelationPlotCollection(SignalHeightCorrelationPlot::SensorType type)
  : PlotCollection(Enums::SignalHeightTracker)
{
  Q_ASSERT(type == SignalHeightCorrelationPlot::Temperature);
  QStringList moduleIDs;
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tracker) {
      unsigned short id = element->id();
      moduleIDs.append(qPrintable(QString("0x%1").arg(id, 0, 16)));
      setTitle("temperature correlation - all channels");
      const QString& title = QString("signal height temperature correlation 0x%1").arg(id, 0, 16);
      const unsigned int nTemperatureBins = 10;
      const double minTemperature = -5;
      const double maxTemperature = 15;
      const unsigned int nSignalHeightBins = 70;
      const double minSignalHeight = 0;
      const double maxSignalHeight = 7000;
      TH2D* histogram = new TH2D(qPrintable(title), qPrintable(title), nTemperatureBins, minTemperature, maxTemperature, nSignalHeightBins, minSignalHeight, maxSignalHeight);
      histogram->GetXaxis()->SetTitle("temperature /  #circC");
      histogram->GetYaxis()->SetTitle("signal height / adc counts");
      addPlot(new SignalHeightCorrelationPlot(id, type, histogram));
    }
  }
  TrackerSipmSelectionWidget* widget = new TrackerSipmSelectionWidget(moduleIDs);
  secondaryWidget()->layout()->addWidget(widget);
  connect(widget, SIGNAL(sipmChanged(int)), this, SLOT(selectPlot(int)));
}


SignalHeightCorrelationPlotCollection::SignalHeightCorrelationPlotCollection(SignalHeightCorrelationPlot::SensorType type, const QDateTime& first, const QDateTime& last)
  : PlotCollection(Enums::SignalHeightTracker)
{
  Q_ASSERT(type == SignalHeightCorrelationPlot::Time);
  QStringList moduleIDs;
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tracker) {
      unsigned short id = element->id();
      moduleIDs.append(qPrintable(QString("0x%1").arg(id, 0, 16)));
      setTitle("time correlation - all channels");
      const QString& title = QString("signal height time correlation 0x%1").arg(id, 0, 16);
      int t1 = first.toTime_t();
      t1-= (t1 % 60) + 60;
      int t2 = last.toTime_t();
      t2+= 120 - (t2 % 60);
      const unsigned int nTimeBins = 10;
      const unsigned int nSignalHeightBins = 35;
      const double minSignalHeight = 0;
      const double maxSignalHeight = 7000;
      TH2D* histogram = new TH2D(qPrintable(title), qPrintable(title), nTimeBins, t1, t2, nSignalHeightBins, minSignalHeight, maxSignalHeight);
      histogram->GetXaxis()->SetTimeDisplay(1);
      histogram->GetXaxis()->SetTimeFormat("%d-%H:%M");
      histogram->GetXaxis()->SetTitle("time");
      histogram->GetYaxis()->SetTitle("signal height / adc counts");
      addPlot(new SignalHeightCorrelationPlot(id, type, histogram));
    }
  }
  TrackerSipmSelectionWidget* widget = new TrackerSipmSelectionWidget(moduleIDs);
  secondaryWidget()->layout()->addWidget(widget);
  connect(widget, SIGNAL(sipmChanged(int)), this, SLOT(selectPlot(int)));
}

SignalHeightCorrelationPlotCollection::~SignalHeightCorrelationPlotCollection()
{
}


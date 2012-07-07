#include "ECALSignalHeight.hh"

#include "SimpleEvent.hh"
#include "Track.hh"
#include "AdditionalHit.hh"
#include "ProjectionControlWidget.hh"

#include <QSpinBox>

#include <TH2.h>
#include <TAxis.h>

ECALSignalHeight::ECALSignalHeight()
  : AnalysisPlot(Enums::Occupancy)
  , H2DProjectionPlot()
{
  setTitle("ecal signal height");

  QVector<int> ids;
  for (int i = 0; i < 12; ++i)
    ids << (0x7400 | i);
  for (int i = 0; i < 12; ++i)
    ids << (0x7500 | i);

  TH2D* histogram = new TH2D(qPrintable(title()), "", ids.size(), 0, ids.size(), 70, -100, 600);
  int bin = 1;
  foreach(int id, ids) {
    histogram->GetXaxis()->SetBinLabel(bin, qPrintable(QString::number(id, 16)));
    m_bins.insert(id, bin);
    ++bin;
  }
  addHistogram(histogram);

  setAxisTitle("id", "signal / ADC counts", "");
  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good);
  controlWidget()->spinBox()->setMaximum(24);
}

ECALSignalHeight::~ECALSignalHeight()
{}

void ECALSignalHeight::processEvent(const AnalyzedEvent* event)
{
  const SimpleEvent* simpleEvent = event->simpleEvent();
  std::vector<AdditionalHit*>::const_iterator it = simpleEvent->additionalHits().begin();
  std::vector<AdditionalHit*>::const_iterator end = simpleEvent->additionalHits().end();
  for (; it != end; ++it) {
    int id = (*it)->detId();
    QMap<unsigned short, int>::Iterator binIterator = m_bins.find(id);
      if (binIterator != m_bins.end()) {
        double binCenter = histogram()->GetXaxis()->GetBinCenter(binIterator.value());
        histogram()->Fill(binCenter, (*it)->signalHeight());
    }
  }
}

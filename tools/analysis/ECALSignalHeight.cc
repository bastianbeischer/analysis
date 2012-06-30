#include "ECALSignalHeight.hh"
#include "BrokenLine.hh"

#include "ParticleInformation.hh"
#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ECALHit.hh"

#include <TH2.h>
#include <TAxis.h>

ECALSignalHeight::ECALSignalHeight()
  : AnalysisPlot(Enums::Occupancy)
  , H2DPlot()
{
  setTitle("ecal signal height");

  QVector<int> ids;
  for (int i = 0; i < 12; ++i)
    ids << (0x7400 | i);
  for (int i = 0; i < 12; ++i)
    ids << (0x7500 | i);
  
  TH2D* histogram = new TH2D(qPrintable(title()), "", ids.size(), 0, ids.size(), 600, -1000, 5000);
  int bin = 1;
  foreach(int id, ids) {
    histogram->GetXaxis()->SetBinLabel(bin, qPrintable(QString::number(id, 16)));
    m_bins.insert(id, bin);
    ++bin;
  }
  addHistogram(histogram);

  setAxisTitle("id", "signal / ADC counts", "");
  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good);
}

ECALSignalHeight::~ECALSignalHeight()
{}

void ECALSignalHeight::processEvent(const AnalyzedEvent* event)
{
  const SimpleEvent* simpleEvent = event->simpleEvent();
  std::vector<ECALHit*>::const_iterator it = simpleEvent->ecalHits().begin();
  std::vector<ECALHit*>::const_iterator end = simpleEvent->ecalHits().end();
  for (; it != end; ++it) {
    int id = (*it)->detId();
    QMap<unsigned short, int>::Iterator binIterator = m_bins.find(id);
      if (binIterator != m_bins.end()) {
        histogram()->Fill(binIterator.value(), (*it)->signalHeight());
    }
  }
} 

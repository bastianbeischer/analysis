#include "PMTSignalHeight.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "AdditionalHit.hh"

#include <TH1.h>
#include <TAxis.h>

PMTSignalHeight::PMTSignalHeight(int id)
  : AnalysisPlot(Enums::Testbeam)
  , H1DPlot()
  , m_id(id)
{
  setTitle("PMT signal height");

  QString histogramTitle = title() + " 0x" + QString::number(id, 16);
  addHistogram(new TH1D(qPrintable(histogramTitle), "", 4096, -4096, 4096));
  setAxisTitle("signal height / ADC counts", "");
  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good);
}

PMTSignalHeight::~PMTSignalHeight()
{}

void PMTSignalHeight::processEvent(const AnalyzedEvent* event)
{
  const SimpleEvent* simpleEvent = event->simpleEvent();
  std::vector<AdditionalHit*>::const_iterator it = simpleEvent->additionalHits().begin();
  std::vector<AdditionalHit*>::const_iterator end = simpleEvent->additionalHits().end();
  for (; it != end; ++it) {
    if ((*it)->detId() == m_id) {
      Q_ASSERT((*it)->type() == AdditionalHit::PMT);
      histogram()->Fill((*it)->signalHeight());
    }
  }
}

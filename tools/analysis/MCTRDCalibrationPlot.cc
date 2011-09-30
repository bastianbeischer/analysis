#include "MCTRDCalibrationPlot.hh"

#include <QDebug>

#include <TH2D.h>

#include "SimpleEvent.hh"
#include "MCSimpleEventDigi.hh"
#include "MCDigiSignal.hh"
#include "Cluster.hh"

MCTRDCalibrationPlot::MCTRDCalibrationPlot()
  : AnalysisPlot(AnalysisTopic::MonteCarloTRD)
  , H2DPlot()
{
  setTitle("MC TRD Calibration");
  TH2D* histogram = new TH2D(qPrintable(title()), "", 1000, -5, 100, 1000, -5, 100);
  setAxisTitle("MC energy deposition / keV", "MC Signal ADCCs", "");
  addHistogram(histogram);
}

void MCTRDCalibrationPlot::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  //qDebug("new event");
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;

  const MCEventInformation* mcInfo = event->MCInformation();

  if(!mcInfo)
    return;


  const std::vector<const MCSimpleEventDigi*> mcDigis = mcInfo->mcDigis();
  const std::vector<Hit*> hits = event->hits();
  QVector<Hit*> trdHits;

  //get all trd channel hits:
  std::vector<Hit*>::const_iterator itHit;
  for (itHit = hits.begin(); itHit != hits.end(); ++itHit) {
    Hit* hit = *itHit;
    if (hit->type() != Hit::trd)
      continue;
    Cluster* cluster = static_cast<Cluster*>(hit);
    const std::vector<Hit*>& subHits = cluster->hits();
    const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      trdHits << *it;
      //qDebug("trd hit id = 0x%x at pos (%f %f %f)", (*it)->detId(), (*it)->position().x(), (*it)->position().y(), (*it)->position().z());
    }
  }

  //qDebug("trdhits: %i", trdHits.size());

  std::vector<const MCSimpleEventDigi*>::const_iterator itDigi;
  for (itDigi = mcDigis.begin(); itDigi != mcDigis.end(); ++itDigi) {
    if ((*itDigi)->type() != Hit::trd)
      continue;
    double enDep = (*itDigi)->signalSum();
    unsigned short channelID = (*itDigi)->detID();

    //find corresponding hit
    double signal = 0.;
    QVector<Hit*>::const_iterator itHit;
    for (itHit = trdHits.begin(); itHit != trdHits.end(); ++itHit) {
      if (channelID == (*itHit)->detId()) {
        signal = (*itHit)->signalHeight();
        break;
      }
    }

    //qDebug("Filling %f, %f", enDep, signal);
    histogram()->Fill(enDep, signal);
  }

}

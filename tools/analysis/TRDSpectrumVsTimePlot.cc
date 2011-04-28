#include "TRDSpectrumVsTimePlot.hh"

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"

#include "TRDCalculations.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QDateTime>
#include <QString>
#include <math.h>

TRDSpectrumVsTimePlot::TRDSpectrumVsTimePlot(unsigned short id, TRDSpectrumType spectrumType, QDateTime first, QDateTime last, double lowerMom, double upperMom) :
  AnalysisPlot(AnalysisPlot:: SignalHeightTRD),
  H2DPlot(),
  m_id(id),
  m_spectrumType(spectrumType),
  m_lowerMomentum(lowerMom),
  m_upperMomentum(upperMom)
{
  QString strType;
  switch(m_spectrumType){
  case TRDSpectrumVsTimePlot::completeTRD:
    strType = "time vs complete TRD";
    break;
  case TRDSpectrumVsTimePlot::module:
    strType = "time vs module";
    break;
  case TRDSpectrumVsTimePlot::channel:
    strType = "time vs channel";
    break;
  }

  if(m_spectrumType == TRDSpectrumVsTimePlot::completeTRD)
    setTitle(strType + QString(" spectrum (%1 GeV to %2 Gev)").arg(m_lowerMomentum).arg(m_upperMomentum));
  else
    setTitle(strType + QString(" spectrum 0x%1 (%2 GeV to %3 Gev)").arg(m_id,0,16).arg(m_lowerMomentum).arg(m_upperMomentum));

  int t1 = first.toTime_t();
  t1-= (t1 % 60) + 60;
  int t2 = last.toTime_t();
  t2+= 120 - (t2 % 60);
  const unsigned int nTimeBins = qMin((t2 - t1) / 60, 500);
  const unsigned int nSignalHeightBins = 100;
  const double minSignalHeight = 0;
  const double maxSignalHeight = 20;

  TH2D* histogram = new TH2D(qPrintable(title()),qPrintable(title() + ";ADCCs per length in tube / (1/mm);entries vs time"), nTimeBins,t1,t2,nSignalHeightBins,minSignalHeight,maxSignalHeight);
  histogram->GetXaxis()->SetTimeDisplay(1);
  histogram->GetXaxis()->SetTimeFormat("%d-%H:%M");
  setAxisTitle("Time", "ADC Counts", "");
  addHistogram(histogram);
}

TRDSpectrumVsTimePlot::~TRDSpectrumVsTimePlot()
{
}

void TRDSpectrumVsTimePlot::processEvent(const QVector<Hit*>& , Particle* particle, SimpleEvent* event)
{
  const Track* track = particle->track();

   //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  if (track->chi2() / track->ndf() > 10)
    return;

  //check if track was inside of magnet
  if (!(particle->information()->flags() & ParticleInformation::InsideMagnet))
    return;

  //get the reconstructed momentum
  double rigidity = track->rigidity(); //GeV

  if(rigidity < m_lowerMomentum || rigidity > m_upperMomentum)
    return;

  //TODO: check for off track hits ?!?
  unsigned int nTrdHits = 0;
  const QVector<Hit*>::const_iterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    if ((*it)->type() == Hit::trd)
      nTrdHits++;
  }

  if (nTrdHits < 6)
    return;

  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Hit* hit = *it;
    if (hit->type() != Hit::trd)
      continue;

    Cluster* cluster = static_cast<Cluster*>(hit);
    std::vector<Hit*>& subHits = cluster->hits();
    const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      Hit* subHit = *it;
      //check if the id of the plot has been hit (difference between module mode and channel mode
      if(m_spectrumType == TRDSpectrumVsTimePlot::completeTRD ||  // one spectrum for whole trd
         (m_spectrumType == TRDSpectrumVsTimePlot::module && (subHit->detId() - subHit->channel()) == m_id) ||  // spectrum per module
         (m_spectrumType == TRDSpectrumVsTimePlot::channel && subHit->detId() == m_id)) {  //spectrum per channel
        double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
        if(distanceInTube > 0)
          histogram(0)->Fill(event->time(), subHit->signalHeight() / (distanceInTube));
      }
    }
  }
}



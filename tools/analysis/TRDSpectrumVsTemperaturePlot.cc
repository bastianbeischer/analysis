#include "TRDSpectrumVsTemperaturePlot.hh"

#include "Particle.hh"
#include "ParticleInformation.hh"
#include "Track.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "TRDCalculations.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <math.h>

TRDSpectrumVsTemperaturePlot::TRDSpectrumVsTemperaturePlot(unsigned int id, TRDSpectrumPlot::TRDSpectrumType spectrumType, double lowerMom, double upperMom):
  AnalysisPlot(AnalysisPlot:: SignalHeightTRD),
  H2DPlot(),
  m_id(id),
  m_spectrumType(spectrumType),
  m_lowerMomentum(lowerMom),
  m_upperMomentum(upperMom)
{
  QString strType;
  switch(m_spectrumType)
  {
    case TRDSpectrumPlot::completeTRD:
      strType = "temperature vs complete TRD";
    break;
    case TRDSpectrumPlot::module:
      strType = "temperature vs module";
    break;
    case TRDSpectrumPlot::channel:
      strType = "temperature vs channel";
    break;
  }

  if(m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + QString(" spectrum (%1 GeV to %2 Gev)").arg(m_lowerMomentum).arg(m_upperMomentum));
  else
    setTitle(strType + QString(" spectrum 0x%1 (%2 GeV to %3 Gev)").arg(m_id,0,16).arg(m_lowerMomentum).arg(m_upperMomentum));


  const unsigned int nTemperatureBins = 100;
  const double minTemperature = -25;
  const double maxTemperature = 35;
  const unsigned int nSpecBins = 100;
  const double minSpec = 0;
  const double maxSpec = 20;

  TH2D* histogram = new TH2D(qPrintable(title()),qPrintable(title() + ";ADCCs per length in tube / (1/mm);entries vs Temperature"), nTemperatureBins, minTemperature, maxTemperature, nSpecBins, minSpec, maxSpec);
  setAxisTitle("temperature /  #circC", "ADCC's", "");
  addHistogram(histogram);
}

TRDSpectrumVsTemperaturePlot::~TRDSpectrumVsTemperaturePlot()
{
}

void TRDSpectrumVsTemperaturePlot::processEvent(const QVector<Hit*>& , Particle* particle, SimpleEvent* event)
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

  // TODO: temp sensormap
  double mean = 0.;
  int count = 0;
  for (unsigned int i = SensorTypes::TRD_TUBE_TOP_HOT_TEMP; i <= SensorTypes::TRD_TUBE_BOTTOM_COLD_TEMP; i++) {
    mean += event->sensorData((SensorTypes::Type)i);
    count++;
  }
  mean /= count;
 
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
      if(m_spectrumType == TRDSpectrumPlot::completeTRD ||  // one spectrum for whole trd
         (m_spectrumType == TRDSpectrumPlot::module && (subHit->detId() - subHit->channel()) == m_id) ||  // spectrum per module
         (m_spectrumType == TRDSpectrumPlot::channel && subHit->detId() == m_id)) {  //spectrum per channel
        double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
        if(distanceInTube > 0)
          histogram(0)->Fill(mean, subHit->signalHeight() / (distanceInTube));
      }
    }
  }
}

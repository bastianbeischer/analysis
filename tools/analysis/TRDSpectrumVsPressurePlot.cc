#include "TRDSpectrumVsPressurePlot.hh"

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

TRDSpectrumVsPressurePlot::TRDSpectrumVsPressurePlot(unsigned int id, TRDSpectrumPlot::TRDSpectrumType spectrumType, double lowerMom, double upperMom):
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
      strType = "complete TRD vs pressure";
    break;
    case TRDSpectrumPlot::module:
      strType = "module vs pressure";
    break;
    case TRDSpectrumPlot::channel:
      strType = "channel vs pressure";
    break;
  }

  if(m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + QString(" spectrum (%1 GeV to %2 GeV)").arg(m_lowerMomentum).arg(m_upperMomentum));
  else
    setTitle(strType + QString(" spectrum 0x%1 (%2 GeV to %3 GeV)").arg(m_id,0,16).arg(m_lowerMomentum).arg(m_upperMomentum));


  const unsigned int nPressureBins = 200;
  const double minPressure = 1070;
  const double maxPressure = 1115;
  const unsigned int nSpecBins = 200;
  const double minSpec = 0;
  const double maxSpec = 20;

  TH2D* histogram = new TH2D(qPrintable(title()),"", nPressureBins, minPressure, maxPressure, nSpecBins, minSpec, maxSpec);
  setAxisTitle("pressure /  mBar", "ADCCs per length", "");
  addHistogram(histogram);
}

TRDSpectrumVsPressurePlot::~TRDSpectrumVsPressurePlot()
{
}

void TRDSpectrumVsPressurePlot::processEvent(const QVector<Hit*>& , Particle* particle, SimpleEvent* event)
{
  const Track* track = particle->track();
  const ParticleInformation::Flags pFlags = particle->information()->flags();

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  if (pFlags & ParticleInformation::Chi2Good)
    return;

  //check if straight line fit has been used:
  if (! (track->type() == Track::StraightLine)){
    //check if track was inside of magnet
    if (!(pFlags & ParticleInformation::InsideMagnet))
      return;

    //get the reconstructed momentum
    double rigidity = track->rigidity(); //GeV

    if(rigidity < m_lowerMomentum || rigidity > m_upperMomentum)
      return;
  }


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
  double pressure = event->sensorData(SensorTypes::TRD_PRESSURE);
 
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
        double distanceInTube = TRDSpectrumPlot::fixedMeanLengthInTube; //default length in trd tube, if no real calcultaion is performed
        if(TRDSpectrumPlot::calculateLengthInTube)
            distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
        if(distanceInTube > 0)
          histogram(0)->Fill(pressure, subHit->signalHeight() / (distanceInTube));
      }
    }
  }
}

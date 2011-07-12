#include "TRDSpectrumVsPressurePlot.hh"

#include "Particle.hh"
#include "ParticleInformation.hh"
#include "Track.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "TRDCalculations.hh"
#include "Settings.hh"
#include "SettingsManager.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <math.h>

TRDSpectrumVsPressurePlot::TRDSpectrumVsPressurePlot(unsigned short id, TRDSpectrumPlot::TRDSpectrumType spectrumType, double lowerMom, double upperMom):
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

  if (m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + QString(" spectrum (%1 GeV to %2 GeV)").arg(m_lowerMomentum).arg(m_upperMomentum));
  else
    setTitle(strType + QString(" spectrum 0x%1 (%2 GeV to %3 GeV)").arg(m_id,0,16).arg(m_lowerMomentum).arg(m_upperMomentum));


  const unsigned int nPressureBins = 200;
  const double minPressure = 1070;
  const double maxPressure = 1115;
  int nBins = TRDSpectrumPlot::spectrumDefaultBins;
  double lowerBound = 1e-3;
  double upperBound = TRDSpectrumPlot::spectrumUpperLimit();
  double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
  double p[nBins+1];
  for (int i = 0; i < nBins+1; i++) {
    p[i] = pow(lowerBound, delta*i+1);
  }

  TH2D* histogram = new TH2D(qPrintable(title()),"", nPressureBins, minPressure, maxPressure, nBins, p);
  setAxisTitle("pressure /  mBar", TRDSpectrumPlot::xAxisTitle(), "");
  addHistogram(histogram);
}

TRDSpectrumVsPressurePlot::~TRDSpectrumVsPressurePlot()
{
}

void TRDSpectrumVsPressurePlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  if (!TRDSpectrumPlot::globalTRDCuts(hits, particle, event))
      return;

  // TODO: temp sensormap
  double pressure = event->sensorData(SensorTypes::TRD_PRESSURE_SMOOTHED);
 
  //now get all relevant energy deposition for this specific plot and all length
  QList<double> lengthList;
  QList<double> signalList;

  const Track* track = particle->track();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != track->hits().end(); ++it) {
    Hit* hit = *it;
    if (hit->type() != Hit::trd)
      continue;
    Cluster* cluster = static_cast<Cluster*>(hit);
    std::vector<Hit*>& subHits = cluster->hits();
    const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      Hit* subHit = *it;
      //check if the id of the plot has been hit (difference between module mode and channel mode
      if (m_spectrumType == TRDSpectrumPlot::completeTRD ||  // one spectrum for whole trd
         (m_spectrumType == TRDSpectrumPlot::module && (subHit->detId() - subHit->channel()) == m_id) ||  // spectrum per module
         (m_spectrumType == TRDSpectrumPlot::channel && subHit->detId() == m_id)) {  //spectrum per channel
        double distanceInTube = 1.; //default length in trd tube, if no real calcultaion is performed
        if (TRDSpectrumPlot::calculateLengthInTube)
            distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
        if (distanceInTube > 0) {
          signalList << hit->signalHeight();
          lengthList << distanceInTube;
        }
      } // fits into category
    } // subhits in cluster
  } // all hits

  /* now fill the mean of all gathered data
      - one value for a single tube
      - normally also one value for a module (except no length is calculated and 2 tubes show a signal)
      - several signals for the complete trd
  */

  //check again if the trdhits are still on the fitted track and fullfill the minTRDLayerCut
  unsigned int hitsWhichAreOnTrack = signalList.size();
  if (m_spectrumType == TRDSpectrumPlot::completeTRD && hitsWhichAreOnTrack < TRDSpectrumPlot::minTRDLayerCut)
    return;

  for (int i = 0; i < signalList.size(); ++i) {
    double value = signalList.at(i) / lengthList.at(i);
    int iGlobalBin = histogram()->FindBin(pressure, value);
    int iXBin, iYBin, iZBin;
    histogram()->GetBinXYZ(iGlobalBin, iXBin, iYBin, iZBin);
    double width = histogram()->GetYaxis()->GetBinWidth(iYBin);
    double weight = 1./width;
    histogram()->Fill(pressure, value, weight);
  }

}

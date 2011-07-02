#include "TRDSpectrumVsTemperaturePlot.hh"

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

TRDSpectrumVsTemperaturePlot::TRDSpectrumVsTemperaturePlot(unsigned short id, TRDSpectrumPlot::TRDSpectrumType spectrumType)
  : AnalysisPlot(AnalysisPlot:: SignalHeightTRD)
  , H2DPlot()
  , m_id(id)
  , m_spectrumType(spectrumType)
{
  QString strType;
  switch(m_spectrumType)
  {
    case TRDSpectrumPlot::completeTRD:
      strType = "complete TRD vs temperature";
    break;
    case TRDSpectrumPlot::module:
      strType = "module vs temperature";
    break;
    case TRDSpectrumPlot::channel:
      strType = "channel vs temperature";
    break;
  }

  if (m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + QString(" spectrum"));
  else
    setTitle(strType + QString(" spectrum 0x%1").arg(m_id,0,16));


  const unsigned int nTemperatureBins = 200;
  const double minTemperature = 26;
  const double maxTemperature = 34;
  int nBins = TRDCalculations::spectrumDefaultBins;
  double lowerBound = 1e-3;
  double upperBound = TRDCalculations::spectrumUpperLimit();
  double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
  double p[nBins+1];
  for (int i = 0; i < nBins+1; i++) {
    p[i] = pow(lowerBound, delta*i+1);
  }

  TH2D* histogram = new TH2D(qPrintable(title()),"", nTemperatureBins, minTemperature, maxTemperature, nBins, p);
  setAxisTitle("temperature /  #circC", TRDCalculations::xAxisTitle(), "");
  addHistogram(histogram);
}

TRDSpectrumVsTemperaturePlot::~TRDSpectrumVsTemperaturePlot()
{
}

void TRDSpectrumVsTemperaturePlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  if (!TRDCalculations::globalTRDCuts(hits, particle, event))
      return;

  // TODO: temp sensormap
  double meanTemperature = 0.;
  int count = 0;
  for (unsigned int i = SensorTypes::TRD_TUBE_TOP_HOT_TEMP; i <= SensorTypes::TRD_TUBE_BOTTOM_COLD_TEMP; i++) {
    meanTemperature += event->sensorData((SensorTypes::Type)i);
    count++;
  }
  meanTemperature /= count;

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
          if (TRDCalculations::calculateLengthInTube)
              distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(subHit, track);
          if (distanceInTube > 0) {
            signalList << subHit->signalHeight();
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
    if (m_spectrumType == TRDSpectrumPlot::completeTRD && hitsWhichAreOnTrack < TRDCalculations::minTRDLayerCut)
      return;

    for (int i = 0; i < signalList.size(); ++i) {
      double value = signalList.at(i) / lengthList.at(i);
      int iGlobalBin = histogram()->FindBin(meanTemperature, value);
      int iXBin, iYBin, iZBin;
      histogram()->GetBinXYZ(iGlobalBin, iXBin, iYBin, iZBin);
      double width = histogram()->GetYaxis()->GetBinWidth(iYBin);
      double weight = 1./width;
      histogram()->Fill(meanTemperature, value, weight);
    }
}

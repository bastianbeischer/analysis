#include "TRDSpectrumVsTimePlot.hh"
#include "TRDSpectrumPlot.hh"

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Settings.hh"
#include "SettingsManager.hh"

#include "TRDCalculations.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QDateTime>
#include <QString>
#include <math.h>

TRDSpectrumVsTimePlot::TRDSpectrumVsTimePlot(QDateTime first, QDateTime last, unsigned short id, TRDSpectrumPlot::TRDSpectrumType spectrumType)
  : AnalysisPlot(AnalysisPlot:: SignalHeightTRD)
  , H2DPlot()
  , m_id(id)
  , m_spectrumType(spectrumType)
{
  QString strType;
  switch(m_spectrumType){
  case TRDSpectrumPlot::completeTRD:
    strType = "complete TRD vs time";
    break;
  case TRDSpectrumPlot::module:
    strType = "module vs time";
    break;
  case TRDSpectrumPlot::channel:
    strType = "channel vs time";
    break;
  }

  if(m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + " spectrum");
  else
    setTitle(strType + QString(" spectrum 0x%1").arg(m_id,0,16));


  int secsPerBin = 3600*2;

  int t1 = first.toTime_t();
  int t2 = last.toTime_t();
  const unsigned int nTimeBins = qMin((t2 - t1) / secsPerBin, 500);
  int nBins = TRDSpectrumPlot::spectrumDefaultBins;
  double lowerBound = 1e-3;
  double upperBound = TRDSpectrumPlot::spectrumUpperLimit();
  double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
  double p[nBins+1];
  for (int i = 0; i < nBins+1; i++) {
    p[i] = pow(lowerBound, delta*i+1);
  }

  TH2D* histogram = new TH2D(qPrintable(title()),"", nTimeBins,t1,t2, nBins, p);
  histogram->Sumw2();
  histogram->GetXaxis()->SetTimeDisplay(1);
  histogram->GetXaxis()->SetTimeFormat("%d-%H:%M");
  histogram->GetXaxis()->SetTimeOffset(3600, "gmt"); //dont understand this, but works at testbeam
  histogram->GetXaxis()->SetTimeDisplay(1);
  setAxisTitle("Time", TRDSpectrumPlot::xAxisTitle(), "");
  addHistogram(histogram);
}

TRDSpectrumVsTimePlot::~TRDSpectrumVsTimePlot()
{
}

void TRDSpectrumVsTimePlot::processEvent(const QVector<Hit*>& hits, const Particle* const particle, const SimpleEvent* const event)
{
  //use the global cuts defined in the TRDSpectrumPlot class
  if ( ! TRDSpectrumPlot::globalTRDCuts(hits, particle, event))
      return;

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
      if(m_spectrumType == TRDSpectrumPlot::completeTRD ||  // one spectrum for whole trd
         (m_spectrumType == TRDSpectrumPlot::module && (subHit->detId() - subHit->channel()) == m_id) ||  // spectrum per module
         (m_spectrumType == TRDSpectrumPlot::channel && subHit->detId() == m_id)) {  //spectrum per channel
        double distanceInTube = 1.; //default length in trd tube, if no real calcultaion is performed
        if(TRDSpectrumPlot::calculateLengthInTube)
            distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
        if(distanceInTube > 0) {
          signalList << hit->signalHeight();
          lengthList << distanceInTube;
        }
      }
    }
  }

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
    int iGlobalBin = histogram()->FindBin(event->time(), value);
    int iXBin, iYBin, iZBin;
    histogram()->GetBinXYZ(iGlobalBin, iXBin, iYBin, iZBin);
    double width = histogram()->GetYaxis()->GetBinWidth(iYBin);
    double weight = 1./width;
    histogram()->Fill(event->time(), value, weight);
  }

}



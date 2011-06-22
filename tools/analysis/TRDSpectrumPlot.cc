#include "TRDSpectrumPlot.hh"

#include <TH1D.h>

#include <TMarker.h>

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "TRDCalculations.hh"
#include "Corrections.hh"

#include <math.h>

#include <QList>

const bool TRDSpectrumPlot::calculateLengthInTube = true;
const int TRDSpectrumPlot::spectrumDefaultBins = 300;
const unsigned int TRDSpectrumPlot::minTRDLayerCut = 6;

TRDSpectrumPlot::TRDSpectrumPlot(unsigned short id, TRDSpectrumType spectrumType) :
  AnalysisPlot(AnalysisPlot::SignalHeightTRD),
  H1DPlot(),
  m_id(id),
  m_spectrumType(spectrumType),
  m_landauFitRange_lower(2./3./100. *TRDSpectrumPlot::spectrumUpperLimit()),
  m_landauFitRange_upper(0.2 *TRDSpectrumPlot::spectrumUpperLimit()),
  m_fitRangeMarker_lower(new TMarker(m_landauFitRange_lower, 0,2)),
  m_fitRangeMarker_upper(new TMarker(m_landauFitRange_upper, 0,2))
{
  QString strType;
  switch(m_spectrumType){
  case TRDSpectrumPlot::completeTRD:
    strType = "complete TRD";
    break;
  case TRDSpectrumPlot::module:
    strType = "Module";
    break;
  case TRDSpectrumPlot::channel:
    strType = "Channel";
    break;
  }

  if(m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + QString(" spectrum"));
  else
    setTitle(strType + QString(" spectrum 0x%1").arg(m_id,0,16));

  //initialize fit function:
  m_landauFit = new TF1(qPrintable(title() + "LandauFit"),"landau", m_landauFitRange_lower, m_landauFitRange_upper);
  m_landauFit->SetLineColor(kRed);

  m_fitRangeMarker_lower->SetMarkerColor(kRed);
  m_fitRangeMarker_upper->SetMarkerColor(kRed);

  int nBins = TRDSpectrumPlot::spectrumDefaultBins;
  double lowerBound = 1e-3;
  double upperBound = TRDSpectrumPlot::spectrumUpperLimit();
  double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
  double p[nBins+1];
  for (int i = 0; i < nBins+1; i++) {
    p[i] = pow(lowerBound, delta*i+1);
  }

  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, p);
  histogram->Sumw2();
  setAxisTitle(TRDSpectrumPlot::xAxisTitle(), "entries");
  addHistogram(histogram, H1DPlot::HIST);
  setDrawOption(H1DPlot::HIST);
}

TRDSpectrumPlot::~TRDSpectrumPlot()
{
  delete m_landauFit;
  delete m_fitRangeMarker_lower;
  delete m_fitRangeMarker_upper;
}

bool TRDSpectrumPlot::globalTRDCUts(const QVector<Hit*>&, Particle* particle, SimpleEvent* event)
{
  const Track* track = particle->track();
  const ParticleInformation::Flags pFlags = particle->information()->flags();

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return false;

  if (pFlags & ParticleInformation::Chi2Good)
    return false;

  //get settings if present
  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);

  //check if magnet was installed, if it was or no information was found, check if the particle went through the inner magnet:
  if (!(settings && !(settings->magnet()))){
    //check if track was inside of magnet
    if (!(pFlags & ParticleInformation::InsideMagnet)  )
      return false;
  }

  //count trd hits which belong to the track and those of the event
  unsigned int nTrdHitsOnTrack = 0;
  unsigned int nTotalTRDHits = 0;
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != track->hits().end(); ++it) {
    if ((*it)->type() == Hit::trd)
      ++nTotalTRDHits;
    if (track->hits().contains(*it))
      ++nTrdHitsOnTrack;
  }

  //trd layer cut

  if (nTrdHitsOnTrack < TRDSpectrumPlot::minTRDLayerCut)
    return false;

  if (nTotalTRDHits > nTrdHitsOnTrack)
    return false;

  //passed all cuts
  return true;
}

void TRDSpectrumPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{

  if ( ! TRDSpectrumPlot::globalTRDCUts(hits, particle, event))
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
    int iBin = histogram()->FindBin(value);
    double width = histogram()->GetBinWidth(iBin);
    double weight = 1./width;
    histogram()->Fill(value, weight);
  }



}

void TRDSpectrumPlot::finalize()
{
  if (histogram(0)->GetEntries() > 30) {
    histogram(0)->Fit(m_landauFit,"Q0","",m_landauFitRange_lower,m_landauFitRange_upper);

    m_fitRangeMarker_lower->SetX(m_landauFitRange_lower);
    m_fitRangeMarker_lower->SetY(m_landauFit->Eval(m_landauFitRange_lower));

    m_fitRangeMarker_upper->SetX(m_landauFitRange_upper);
    m_fitRangeMarker_upper->SetY(m_landauFit->Eval(m_landauFitRange_upper));
  }

  //  if (false) {
  //    //write fit results:
  //    Corrections corrections;
  //    if( m_spectrumType == TRDSpectrumPlot::module){
  //      for (int chan = 0; chan < 16; chan++){
  //        unsigned short channelID = m_id | chan ;
  //        double mopvValue = m_landauFit->GetParameter(1) ;
  //        double oldScalingFactor = corrections.trdScalingFactor(channelID) ;
  //        //TODO target value of mopv is 1 or whatever?
  //        double newScalingFactor =  1.0 * oldScalingFactor / mopvValue;
  //        qDebug("setting for 0x%x old TRDScalingFactor from %f to %f", channelID, oldScalingFactor,  newScalingFactor) ;
  //        corrections.setTrdScalingFactor(channelID, newScalingFactor) ;
  //      }
  //    }
  // }
}

void TRDSpectrumPlot::update()
{
}

void TRDSpectrumPlot::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  m_landauFit->Draw("same");
  m_fitRangeMarker_lower->Draw("same");
  m_fitRangeMarker_upper->Draw("same");
}

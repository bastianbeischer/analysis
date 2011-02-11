#include "TRDSpectrumPlot.hh"

#include <TH1D.h>

#include <TMarker.h>

#include "Track.hh"
#include "TrackInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"

#include "TRDCalculations.hh"
#include "Corrections.hh"

TRDSpectrumPlot::TRDSpectrumPlot(AnalysisPlot::Topic topic, unsigned short id, TRDSpectrumType spectrumType, double lowerMom, double upperMom) :
  AnalysisPlot(topic),
  H1DPlot(),
  m_id(id),
  m_spectrumType(spectrumType),
  m_landauFitRange_lower(0.1),
  m_landauFitRange_upper(3.0),
  m_lowerMomentum(lowerMom),
  m_upperMomentum(upperMom),
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
    setTitle(strType + QString(" spectrum (%1 GeV to %2 Gev)").arg(m_lowerMomentum).arg(m_upperMomentum));
  else
    setTitle(strType + QString(" spectrum 0x%1 (%2 GeV to %3 Gev)").arg(m_id,0,16).arg(m_lowerMomentum).arg(m_upperMomentum));

  //initialize fit function:
  m_landauFit = new TF1(qPrintable(title() + "LandauFit"),"landau",0,150);
  m_landauFit->SetLineColor(kRed);

  m_fitRangeMarker_lower->SetMarkerColor(kRed);
  m_fitRangeMarker_upper->SetMarkerColor(kRed);

  TH1D* histogram = new TH1D(qPrintable(title()), qPrintable(title() + ";ADCCs per length in tube / (1/mm);entries"), 50, 0, 15);
  addHistogram(histogram);
}

TRDSpectrumPlot::~TRDSpectrumPlot()
{
  delete m_landauFit;
  delete m_fitRangeMarker_lower;
  delete m_fitRangeMarker_upper;
}

void TRDSpectrumPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*)
{
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  //check if all tracker layers have a hit
  TrackInformation::Flags flags = track->information()->flags();
  //if (!(flags & TrackInformation::AllTrackerLayers))
    //return;

  //check if track was inside of magnet
  if (!(flags & TrackInformation::InsideMagnet))
    return;

  //get the reconstructed momentum
  double p = track->p(); //GeV

  if(p < m_lowerMomentum || p > m_upperMomentum)
    return;

  //loop over all hits and count tracker hits
  //also find all clusters on track
  QVector<Hit*> trdClusterHitsOnTrack;

  //TODO: check for off track hits ?!?
  foreach(Hit* clusterHit, hits){
    if (clusterHit->type() == Hit::trd)
      trdClusterHitsOnTrack.push_back(clusterHit);
  }

  if(trdClusterHitsOnTrack.size() < 6)
    return;

  foreach(Hit* clusterHit, trdClusterHitsOnTrack){
    Cluster* cluster = static_cast<Cluster*>(clusterHit);
    foreach(Hit* hit, cluster->hits()){
      //check if the id of the plot has been hit (difference between module mode and channel mode
      if(m_spectrumType == TRDSpectrumPlot::completeTRD ||  // one spectrum for whole trd
         (m_spectrumType == TRDSpectrumPlot::module && (hit->detId() - hit->channel()) == m_id) ||  // spectrum per module
         (m_spectrumType == TRDSpectrumPlot::channel && hit->detId() == m_id)){  //spectrum per channel
        double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
        if(distanceInTube > 0){
          histogram(0)->Fill(hit->signalHeight() / (distanceInTube));
        }
      }
    }
  }

}

void TRDSpectrumPlot::finalize()
{
  histogram(0)->Fit(m_landauFit,"Q0","",m_landauFitRange_lower,m_landauFitRange_upper);

  m_fitRangeMarker_lower->SetX(m_landauFitRange_lower);
  m_fitRangeMarker_lower->SetY(m_landauFit->Eval(m_landauFitRange_lower));

  m_fitRangeMarker_upper->SetX(m_landauFitRange_upper);
  m_fitRangeMarker_upper->SetY(m_landauFit->Eval(m_landauFitRange_upper));

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

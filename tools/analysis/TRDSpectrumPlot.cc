#include "TRDSpectrumPlot.hh"

#include <TH1D.h>

#include <TMarker.h>

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"

#include "TRDCalculations.hh"
#include "Corrections.hh"

TRDSpectrumPlot::TRDSpectrumPlot(unsigned short id, TRDSpectrumType spectrumType, double lowerMom, double upperMom) :
  AnalysisPlot(AnalysisPlot::SignalHeightTRD),
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
    setTitle(strType + QString(" spectrum (%1 GeV to %2 GeV)").arg(m_lowerMomentum).arg(m_upperMomentum));
  else
    setTitle(strType + QString(" spectrum 0x%1 (%2 GeV to %3 GeV)").arg(m_id,0,16).arg(m_lowerMomentum).arg(m_upperMomentum));

  //initialize fit function:
  m_landauFit = new TF1(qPrintable(title() + "LandauFit"),"landau",0,150);
  m_landauFit->SetLineColor(kRed);

  m_fitRangeMarker_lower->SetMarkerColor(kRed);
  m_fitRangeMarker_upper->SetMarkerColor(kRed);

  TH1D* histogram = new TH1D(qPrintable(title()), "", 50, 0, 15);
  setAxisTitle("ADCCs per length in tube / (1/mm)", "entries");

  addHistogram(histogram);
}

TRDSpectrumPlot::~TRDSpectrumPlot()
{
  delete m_landauFit;
  delete m_fitRangeMarker_lower;
  delete m_fitRangeMarker_upper;
}

void TRDSpectrumPlot::processEvent(const QVector<Hit*>& /*hits*/, Particle* particle, SimpleEvent* event)
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
          histogram(0)->Fill(subHit->signalHeight() / (distanceInTube));
      } // fits into category
    } // subhits in cluster
  } // all hits
}

void TRDSpectrumPlot::finalize()
{
  if (histogram(0)->GetEntries() > 10) {
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

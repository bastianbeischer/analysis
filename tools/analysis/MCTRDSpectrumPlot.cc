#include "MCTRDSpectrumPlot.hh"

#include <TH1D.h>

#include <TCanvas.h>
#include <TLegend.h>


#include "SimpleEvent.hh"
#include "Track.hh"
#include "TrackInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"

#include "TRDCalculations.hh"
#include "Corrections.hh"
#include "RootStyle.hh"

MCTRDSpectrumPlot::MCTRDSpectrumPlot(AnalysisPlot::Topic topic, unsigned short id, TRDSpectrumType spectrumType, double lowerMom, double upperMom)
  : AnalysisPlot(topic)
  , H1DPlot()
  , m_colorCounter(0)
  , m_id(id)
  , m_spectrumType(spectrumType)
  , m_lowerMomentum(lowerMom)
  , m_upperMomentum(upperMom)
{
  QString strType;
  switch(m_spectrumType){
  case MCTRDSpectrumPlot::completeTRD:
    strType = "complete TRD";
    break;
  case MCTRDSpectrumPlot::module:
    strType = "Module";
    break;
  case MCTRDSpectrumPlot::channel:
    strType = "Channel";
    break;
  }

  if(m_spectrumType == MCTRDSpectrumPlot::completeTRD)
    setTitle(QString("MC spectra"));
  else
    setTitle(QString("MC spectra 0x%1").arg(m_id,0,16));

  TLegend* legend = new TLegend(.72, .72, .98, .98);
  legend->SetFillColor(kWhite);
  legend->SetMargin(.7);
  addLegend(legend);

}

MCTRDSpectrumPlot::~MCTRDSpectrumPlot()
{
}

void MCTRDSpectrumPlot::processEvent(const QVector<Hit*>& /*hits*/, Track* track, SimpleEvent* event)
{
  //only accept mc events:
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  if (track->chi2() / track->ndf() > 10)
    return;

  //check if track was inside of magnet
  if (!(track->information()->flags() & TrackInformation::InsideMagnet))
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
      if(m_spectrumType == MCTRDSpectrumPlot::completeTRD ||  // one spectrum for whole trd
         (m_spectrumType == MCTRDSpectrumPlot::module && (subHit->detId() - subHit->channel()) == m_id) ||  // spectrum per module
         (m_spectrumType == MCTRDSpectrumPlot::channel && subHit->detId() == m_id)) {  //spectrum per channel
        double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(hit, track);
        if(distanceInTube > 0)
        {
          //get histo:
          int pdgID = event->MCInformation()->primary()->pdgID;

          TH1D* spectrumHisto = 0;

          QMutexLocker locker(&m_mutex);
          if (m_spectrumMap.contains(pdgID))
            spectrumHisto = m_spectrumMap.value(pdgID);
          else
          {
            spectrumHisto = new TH1D(qPrintable(QString::number(pdgID) + " " + title())
                                     , qPrintable(QString::number(pdgID) +" " + title() + ";ADCCs per length in tube / (1/mm);entries")
                                     , 50, 0, 15);
            spectrumHisto->SetLineColor(RootStyle::rootColor(m_colorCounter++));
            m_spectrumMap.insert(pdgID, spectrumHisto);
            legend()->AddEntry(spectrumHisto, qPrintable(QString::number(pdgID)), "l");
            addHistogram(spectrumHisto);
          }

          spectrumHisto->Fill(subHit->signalHeight() / (distanceInTube));

        }
      } // fits into category
    } // subhits in cluster
  } // all hits
}


void MCTRDSpectrumPlot::finalize()
{
  for (int i = 0; i < numberOfHistograms(); i++)
  {
    if (histogram(i)->Integral("width") > 0)
      histogram(i)->Scale(1./histogram(i)->Integral());
  }
}

void MCTRDSpectrumPlot::draw(TCanvas* canvas)
{
  canvas->cd();
  for (int i = numberOfHistograms()-1; i >= 0; i--)
  {
    TH1D* h = histogram(i);
    if (h == histogram(numberOfHistograms()-1))
      h->Draw();
    else
      h->Draw("SAME");
  }
  RootPlot::draw(canvas);
}

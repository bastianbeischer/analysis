#include "MCTRDSpectrumPlot.hh"

#include <TH1D.h>

#include <TCanvas.h>
#include <TLegend.h>


#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "ParticleDB.hh"
#include "ParticleProperties.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "TRDSpectrumPlot.hh"

#include "TRDCalculations.hh"
#include "Corrections.hh"
#include "RootStyle.hh"

MCTRDSpectrumPlot::MCTRDSpectrumPlot(unsigned short id, TRDSpectrumType spectrumType)
  : AnalysisPlot(AnalysisPlot::MonteCarloTRD)
  , H1DPlot()
  , m_colorCounter(0)
  , m_id(id)
  , m_spectrumType(spectrumType)
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

  setAxisTitle(TRDSpectrumPlot::xAxisTitle(), "entries");

  TLegend* legend = new TLegend(.72, .72, .98, .98);
  legend->SetFillColor(kWhite);
  legend->SetMargin(.7);
  addLegend(legend);

}

MCTRDSpectrumPlot::~MCTRDSpectrumPlot()
{
}

void MCTRDSpectrumPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  //only MC Events
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;

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
      if(m_spectrumType == MCTRDSpectrumPlot::completeTRD ||  // one spectrum for whole trd
         (m_spectrumType == MCTRDSpectrumPlot::module && (subHit->detId() - subHit->channel()) == m_id) ||  // spectrum per module
         (m_spectrumType == MCTRDSpectrumPlot::channel && subHit->detId() == m_id)) {  //spectrum per channel
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
  if (m_spectrumType == MCTRDSpectrumPlot::completeTRD && hitsWhichAreOnTrack < TRDSpectrumPlot::minTRDLayerCut)
    return;

  //get histo:
  int pdgID = event->MCInformation()->primary()->pdgID;

  TH1D* spectrumHisto = 0;

  QMutexLocker locker(&m_mutex);
  if (m_spectrumMap.contains(pdgID))
    spectrumHisto = m_spectrumMap.value(pdgID);
  else
  {
    const ParticleProperties* properties = ParticleDB::instance()->lookupPdgId(pdgID);
    QString particleName = properties->name();
    spectrumHisto = new TH1D(qPrintable(particleName + " " + title()), "", TRDSpectrumPlot::spectrumDefaultBins, 0, TRDSpectrumPlot::spectrumUpperLimit());
    spectrumHisto->SetLineColor(RootStyle::rootColor(m_colorCounter++));
    m_spectrumMap.insert(pdgID, spectrumHisto);
    legend()->AddEntry(spectrumHisto, qPrintable(particleName), "l");
    addHistogram(spectrumHisto);
  }

  double lengthSum = 0.;
  double signalSum = 0.;
  double meanSignalPerLength = 0.;
  for (int i = 0; i < signalList.size(); ++i) {
    signalSum += signalList.at(i);
    lengthSum += lengthList.at(i);
    meanSignalPerLength += signalList.at(i) / lengthList.at(i);
    spectrumHisto->Fill(signalList.at(i) / lengthList.at(i));
  }
  meanSignalPerLength /= signalList.size();
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

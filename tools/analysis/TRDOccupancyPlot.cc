#include "TRDOccupancyPlot.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "Track.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "TRDReconstruction.hh"
#include "TRDSpectrumPlot.hh"

#include <TCanvas.h>
#include <TList.h>
#include <TH2.h>
#include <TAxis.h>
#include <TPaletteAxis.h>

TRDOccupancyPlot::TRDOccupancyPlot(TrdOccupancyType occupancyType, bool onlyOnTrack)
  : AnalysisPlot(Enums::MiscellaneousTRD)
  , H2DPlot()
  , m_occupancyType(occupancyType)
  , m_onlyOnTrack(onlyOnTrack)
{
  QString tempTitle;
  switch(m_occupancyType){
  case TRDOccupancyPlot::numberOfHits:
    tempTitle = "TRD occupancy: number of hits";
    break;
  case TRDOccupancyPlot::sumOfSignalHeights:
    tempTitle = "TRD occupancy: signal sum";
    break;
  case TRDOccupancyPlot::sumOfSignalHeightsNormalizedToHits:
    tempTitle = "TRD occupancy: signal sum normalized to hits";
    break;
  }

  if(m_onlyOnTrack)
    tempTitle += ", only on track";
  else
    tempTitle += ", without any filter";


  setTitle(tempTitle);

  TH2D* histogram = new TH2D(qPrintable(title()), qPrintable(title() + ";x / mm;z /mm"),200,-120,120,200,-550, -250);
  addHistogram(histogram);

  //initialize all needed ellipses
  initializeEllipses();
}

TRDOccupancyPlot::~TRDOccupancyPlot()
{
  qDeleteAll(m_ellipses.values());
  m_ellipses.clear();
}

void TRDOccupancyPlot::processEvent(const QVector<Hit*>& hits, const Particle* const particle, const SimpleEvent* const)
{

  const Track* track = particle->track();
  const QVector<Hit*> trackHits = track->hits();
  const ParticleInformation::Flags pFlags = particle->information()->flags();

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  if (pFlags & ParticleInformation::Chi2Good)
    return;

  //loop over all hits of the event
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* clusterHit = *it;

    //only trd:
    if(clusterHit->type() != Hit::trd)
      continue;

    Cluster* cluster = static_cast<Cluster*>(clusterHit);

    //loop over all subhits of the cluster, in most cases just one
    const std::vector<Hit*>& subHits = cluster->hits();
    const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      Hit* hit = *it;
      //analyse the hit:
      double distanceInTube = 1.;

      if(TRDReconstruction::s_calculateLengthInTube)
          TRDReconstruction::distanceOnTrackThroughTRDTube(hit, track);

      if(!(distanceInTube > 0 && trackHits.contains(clusterHit)) && m_onlyOnTrack)
        continue;

      double signal = hit->signalHeight() / (distanceInTube);

      m_hits[hit->detId()]++;
      m_signalHeightSum[hit->detId()] += signal;
    
      //fill histo
      double value = 0;
      int binToFill = histogram()->FindBin(hit->position().x(), hit->position().z());
      switch(m_occupancyType){
      case TRDOccupancyPlot::numberOfHits:
        value = m_hits[hit->detId()];
        break;
      case TRDOccupancyPlot::sumOfSignalHeights:
        value =  m_signalHeightSum[hit->detId()];
        break;
      case TRDOccupancyPlot::sumOfSignalHeightsNormalizedToHits:
        if(m_hits[hit->detId()] > 0)
          value = m_signalHeightSum[hit->detId()] / (double)m_hits[hit->detId()];
        break;
      }
      histogram()->SetBinContent(binToFill,value);
      // TPaletteAxis* palette = (TPaletteAxis*) histogram()->GetListOfFunctions()->FindObject("palette");
      // if(palette) {
      //   ell->SetFillColor( palette->GetValueColor(value));
      // }
    }
  }
}


void TRDOccupancyPlot::finalize()
{
}

void TRDOccupancyPlot::draw(TCanvas* c)
{
  //qDebug("Draw");
  H2DPlot::draw(c);

  updateEllipses();

  //loop all map entries:
  QMapIterator<unsigned short, TEllipse*> i(m_ellipses);
  while (i.hasNext()) {
    i.next();

    i.value()->Draw("same");
  }
}

void TRDOccupancyPlot::updateEllipses()
{
  //qDebug("update ellipses");
  gPad->Update();
  TPaletteAxis* palette = (TPaletteAxis*) histogram()->GetListOfFunctions()->FindObject("palette");

  if(!palette)
    return;

  //loop all map entries:
  QMapIterator<unsigned short, quint32> i(m_hits);
  while (i.hasNext()) {
    i.next();
    //get value to fill the ellipses with
    double value = 0;
    switch(m_occupancyType){
    case TRDOccupancyPlot::numberOfHits:
      value = i.value();
      break;
    case TRDOccupancyPlot::sumOfSignalHeights:
      value = m_signalHeightSum.value(i.key());
      break;
    case TRDOccupancyPlot::sumOfSignalHeightsNormalizedToHits:
      if(m_hits[i.key()] == 0)
        value = 0;
      else
        value = m_signalHeightSum.value(i.key()) / (double)i.value();
      break;
    }
    TEllipse* ell = m_ellipses.value(i.key());
    ell->SetFillColor(palette->GetValueColor(value));
    //ell->Draw("same");
  }
}

void TRDOccupancyPlot::update()
{
  static int counter = 0;
  if (counter % 10 == 0) {
    updateEllipses();
  }
  counter++;
}

void TRDOccupancyPlot::initializeEllipses()
{
  //create for each trd channel an ellipses
  Setup* setup = Setup::instance();

  const ElementIterator endIt = setup->lastElement();
  for (ElementIterator it = setup->firstElement(); it != endIt; ++it) {
    DetectorElement* element = *it;
    if (element->type() == DetectorElement::trd){
      for(unsigned short tubeNo = 0; tubeNo < 16; tubeNo++){
        unsigned short detID = element->id() | tubeNo;
        TVector3 posTRDChan = setup->configFilePosition("trd",detID);
        m_ellipses.insert(detID, new TEllipse(posTRDChan.x(),posTRDChan.z(), 3.0)) ;
      }
    }
  }
}

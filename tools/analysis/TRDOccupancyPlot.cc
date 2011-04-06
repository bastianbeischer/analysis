#include "TRDOccupancyPlot.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "Track.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"

#include <TCanvas.h>
#include <TList.h>
#include <TH2.h>
#include <TAxis.h>
#include <TPaletteAxis.h>

TRDOccupancyPlot::TRDOccupancyPlot(TrdOccupancyType occupancyType, bool onlyOnTrack)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTRD)
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

void TRDOccupancyPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*)
{
  const QVector<Hit*>& clustersToAnalyze = m_onlyOnTrack ? track->hits() : hits;

  const QVector<Hit*>::const_iterator endIt = clustersToAnalyze.end();
  for (QVector<Hit*>::const_iterator it = clustersToAnalyze.begin(); it != endIt; ++it) {
    Hit* clusterHit = *it;

    //only trd:
    if(clusterHit->type() != Hit::trd)
      continue;

    Cluster* cluster = static_cast<Cluster*>(clusterHit);

    std::vector<Hit*>& subHits = cluster->hits();
    const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      Hit* hit = *it;
      m_hits[hit->detId()]++;
      m_signalHeightSum[hit->detId()] += hit->signalHeight();
    
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
      //   m_mutex.lock();
      //   ell->SetFillColor( palette->GetValueColor(value));
      //   m_mutex.unlock();
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
    m_mutex.lock();
    ell->SetFillColor(palette->GetValueColor(value));
    m_mutex.unlock();
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

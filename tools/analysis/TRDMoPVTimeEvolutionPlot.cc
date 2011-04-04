#include "TRDMoPVTimeEvolutionPlot.hh"

#include <TGraph.h>


#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TLegend.h>

#include "Track.hh"
#include "TrackInformation.hh"
#include "SimpleEvent.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

#include "TRDCalculations.hh"

#include <QDateTime>

#include <QDebug>

TRDMoPVTimeEvolutionPlot::TRDMoPVTimeEvolutionPlot(AnalysisPlot::Topic topic) :
  AnalysisPlot(topic),
  GraphPlot(),
  m_landauFitRange_lower(1.0),
  m_landauFitRange_upper(50.0),
  m_timeBinLength(20*60)
{
  setTitle("MoPV time evolution");

  //add a graph for each module:

  TLegend* legend = new TLegend();
  int color = 0;

  Setup* setup = Setup::instance();
  const ElementIterator endIt = setup->lastElement();
  for (ElementIterator it = setup->firstElement(); it != endIt; ++it) {
    DetectorElement* element = *it;
    if (element->type() == DetectorElement::trd){
      TGraphErrors* g = new TGraphErrors();

      g->SetMarkerStyle(1);
      g->SetLineColor(color++);
      addGraph(g, "L");

      legend->AddEntry(g, qPrintable("Module 0x" + QString::number(element->id(),16)));

      m_mopvGraphs.insert(element->id(),g);
    }
  }
  setAxisTitle("Run", "MoPV");

  addLegend(legend);
}

TRDMoPVTimeEvolutionPlot::~TRDMoPVTimeEvolutionPlot()
{
  for (int i = 0; i < m_binningMap.size(); i++)
    qDeleteAll(m_binningMap.values().at(i));

  //qDeleteAll(m_mopvGraphs);
}

void TRDMoPVTimeEvolutionPlot::processEvent(const QVector<Hit*>& /*hits*/, Track* track, SimpleEvent* event)
{
  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  //check if all tracker layers have a hit
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  //check if track was inside of magnet
  //if (!(flags & TrackSelection::InsideMagnet))
  //    return;


  QDateTime runTime = QDateTime::fromTime_t(event->runStartTime()) ;
  QDateTime eventTime = runTime.addMSecs(event->eventTime()) ;

  unsigned int binValue = eventTime.toTime_t() / (m_timeBinLength) * (m_timeBinLength) + (0.5*m_timeBinLength);

  const QVector<Hit*>::const_iterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Cluster* cluster = static_cast<Cluster*>(*it);
    if (cluster->type() != Hit::trd) 
      continue;
    
    std::vector<Hit*>& subHits = cluster->hits();
    const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
    for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
      Hit* subHit = *it;
      //check if the id of the plot has been hit (difference between module mode and channel mode
      double distanceInTube = TRDCalculations::distanceOnTrackThroughTRDTube(subHit, track);
      if(distanceInTube > 0){

        //now lock, to be threadsafe
        QMutexLocker locker(&m_mutex);

        QMap < unsigned short, TH1* >& histoMapOfRunFile = m_binningMap[binValue] ;
        unsigned int moduleID = subHit->detId() & 0xFFF0;
        if ( !histoMapOfRunFile.contains(moduleID) ){
          //qDebug() << "trdSpectrumHisto_for_run" + QString::number(runFile) + "_module_0x" + QString::number(moduleID,16);
          TH1D* histo = new TH1D(qPrintable("trdSpectrumHisto_for_bin" + QString::number(binValue) + "_module_0x" + QString::number(moduleID,16)) ,
                                 "egal", 50, 0, 50);
          histoMapOfRunFile.insert(moduleID, histo);
          //qDebug("fill %f", subHit->signalHeight() / (distanceInTube));
          histo->Fill(subHit->signalHeight() / (distanceInTube) );
        } else {
          histoMapOfRunFile[moduleID]->Fill(subHit->signalHeight() / (distanceInTube) );
          //qDebug("fill %f", subHit->signalHeight() / (distanceInTube));
        }
        //qDebug("histo now contains entries %f", histoMapOfRunFile[moduleID]->GetEntries());
        m_graphNeedsUpdate = true;
      }
    }
  }
}

void TRDMoPVTimeEvolutionPlot::finalize()
{
  // update();
}

//void TRDMoPVTimeEvolutionPlot::draw(TCanvas* canvas)
//{
//  update();
//  RootPlot::draw(canvas);
//}

void TRDMoPVTimeEvolutionPlot::update()
{
  if(m_graphNeedsUpdate)
    updateGraph();
}


void TRDMoPVTimeEvolutionPlot::updateGraph()
{

  // qDebug("m_mopvGraphs.size() = %i", m_mopvGraphs.size() );
  // qDebug("m_runFileMap.size() = %i", m_binningMap.size() );

  QMutexLocker locker(&m_mutex);

  foreach (unsigned int module, m_mopvGraphs.keys()) {
    unsigned int pointNo = 0;
    foreach (unsigned int run, m_binningMap.keys()) {
      if (m_binningMap.value(run).contains(module)) {
        // qDebug("fitting a landau to hist with entries %f", m_binningMap.value(run).value(module)->GetEntries());
        TFitResultPtr fitResult = m_binningMap.value(run).value(module)->Fit("landau","0QS","",m_landauFitRange_lower,m_landauFitRange_upper) ;
        TGraphErrors* moduleGraph = m_mopvGraphs.value(module);
        int fitStatus = fitResult;
        if ( fitStatus == 0 ) {
          // qDebug("for module 0x%x adding point %i, %f", module, run, fitResult->Parameter(1));
          moduleGraph->SetPoint(pointNo, run, fitResult->Parameter(1));
          moduleGraph->SetPointError(pointNo, 0, fitResult->ParError(1));
          pointNo++ ;
        } else {
          //moduleGraph->SetPoint(pointNo, run, -1.0);
        }
      }
    }
  }

  if (m_drawn) {
    m_multiGraph->GetXaxis()->SetLimits(m_binningMap.keys().first(), m_binningMap.keys().last()) ;
    m_graphNeedsUpdate = false;
  } else {
    m_graphNeedsUpdate = true;
  }
}



#include "TRDEfficiencyPlot.hh"

#include "Setup.hh"
#include "DetectorElement.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "TRDReconstruction.hh"

#include <TCanvas.h>
#include <TList.h>
#include <TH2.h>
#include <TAxis.h>
#include <TPaletteAxis.h>

#include <QDebug>

#include <math.h>

TRDEfficiencyPlot::TRDEfficiencyPlot()
  : AnalysisPlot(AnalysisTopic::MiscellaneousTRD)
  , H2DPlot()
{
  setTitle("TRD efficiency (hits / was on track)");

  TH2D* histogram = new TH2D(qPrintable(title()), qPrintable(title() + ";x / mm;z /mm"),200,-120,120,200,-550, -250);
  addHistogram(histogram);

  //initialize the 2D positions of the trd tubes:
  Setup* setup = Setup::instance();

  const ElementIterator endIt = setup->lastElement();
  for (ElementIterator it = setup->firstElement(); it != endIt; ++it) {
    DetectorElement* element = *it;
    if (element->type() == DetectorElement::trd){
      //loop over the 16 tubes
      for( int i = 0; i < 16; i++){
        int detID = element->id() | i ;
        //get position of trd tube
        TVector3 pos3D = setup->configFilePosition("trd", detID);
        QVector2D pos2D(pos3D.x(), pos3D.z());
        m_trdChanPositions.insert(detID, pos2D);
      }
    }
  }

  //initialize all needed ellipses
  initializeEllipses();

  //connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
  //m_updateTimer.start(3000);
}

TRDEfficiencyPlot::~TRDEfficiencyPlot()
{
  qDeleteAll(m_ellipses.values());
  m_ellipses.clear();
}

void TRDEfficiencyPlot::processEvent(const QVector<Hit*>& /*hits*/, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();
  const ParticleInformation::Flags pFlags = particle->information()->flags();

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  if (pFlags & ParticleInformation::Chi2Good)
    return;

  //TODO: check for off track hits, atm this is Bastians criteria for on track
  const QVector<Hit*>::const_iterator hitsEnd = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::trd) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      //check if event contains trd clusters with more than 2 sub hits
      if (cluster->hits().size() > 2)
        return;
    }
  }

  //TODO: loops are not very efficient (maybe use more breaks or smarter searches in rows)

  //now we loop over all trd tubes and calculate the distance between their wire and the track
  //if the tube is hit by the track (defined by a radius a bit smaller than the tube radius)
  //we check wheter the tube shows a signal or not
  QMapIterator<unsigned short, QVector2D> i(m_trdChanPositions);
  while (i.hasNext()) {
    i.next();
    QVector2D pos2D = i.value();
    //get distance
    double distance = TRDReconstruction::distanceTrackToWire(pos2D, track);
    //check if tube has been punched by track:
    if (fabs(distance) < 1) {
      //was on track +1
      m_wasOnTrack[i.key()]++;
      //now check wheter the tube has seen a signal:
      for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != hitsEnd; ++it) {
        Cluster* cluster = static_cast<Cluster*>(*it);
        if (cluster->type() != Hit::trd)
          continue;

        const std::vector<Hit*>& subHits = cluster->hits();
        const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
        for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
          Hit* subHit = *it;
          if ( i.key() == subHit->detId()) {
            //tube did see a signal +1
            m_hits[i.key()]++;
          }
        }
      }
      //update histo bin:
      unsigned long hits = m_hits.value(i.key());
      unsigned long wasOnTrack = m_wasOnTrack.value(i.key());
      double value = hits / static_cast<double>(wasOnTrack);
      int bin = histogram()->FindBin(pos2D.x(), pos2D.y());
      histogram()->SetBinContent(bin, value);
    }
  }

}


void TRDEfficiencyPlot::finalize()
{
}

void TRDEfficiencyPlot::draw(TCanvas* c)
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

void TRDEfficiencyPlot::updateEllipses()
{
  //qDebug("update ellipses");
  gPad->Update();
  TPaletteAxis* palette = (TPaletteAxis*) histogram()->GetListOfFunctions()->FindObject("palette");

  if(!palette)
    return;

  //loop all map entries:
  QMapIterator<unsigned short, unsigned long> i(m_wasOnTrack);
  while (i.hasNext()) {
    i.next();

    double value = m_hits.value(i.key()) / static_cast<double>(i.value());
    TEllipse* ell = m_ellipses.value(i.key());
    ell->SetFillColor(palette->GetValueColor(value));
  }
}

void TRDEfficiencyPlot::update()
{
  static int counter = 0;
  if (counter % 10 == 0) {
    updateEllipses();
  }
  counter++;
}

void TRDEfficiencyPlot::initializeEllipses()
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
        TEllipse* ell = new TEllipse(posTRDChan.x(),posTRDChan.z(), 3.0);
        m_ellipses.insert(detID, ell) ;
        ell->SetFillColor(kWhite);
      }
    }
  }
}

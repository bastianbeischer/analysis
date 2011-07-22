#include "HitsPlot.hh"

#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "Particle.hh"
#include "Track.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"
#include "StraightLine.hh"
#include "SimpleEvent.hh"
#include "MCSimpleEventParticle.hh"
#include "TOFCluster.hh"
#include "DetectorElement.hh"
#include "Setup.hh"

#include <TCanvas.h>
#include <TList.h>
#include <TLine.h>
#include <TVector3.h>
#include <TGraph.h>
#include <TLatex.h>
#include <TH2D.h>
#include <TBox.h>
#include <THistPainter.h>
#include <TPaletteAxis.h>
#include <TGaxis.h>
#include <TMarker.h>
#include <TStyle.h>

#include <iostream>
#include <algorithm>

HitsPlot::HitsPlot()
  : PerdaixDisplay()
  , EventDestination()
  , m_fitInfo(0)
  , m_drawAllClusters(false)
{
}

HitsPlot::~HitsPlot()
{
  clearHits();

  qDeleteAll(m_trajectoriesXZ);
  qDeleteAll(m_trajectoriesYZ);
}

void HitsPlot::clearHits()
{
  qDeleteAll(m_hits);
  m_hits.clear();
  qDeleteAll(m_lines);
  m_lines.clear();
  qDeleteAll(m_markers);
  m_markers.clear();
  delete m_fitInfo;
  m_fitInfo = 0;
}

void HitsPlot::processEvent(const QVector<Hit*>& hits, const Particle* const particle, const SimpleEvent* const event)
{
  const Track* track = particle->track();

  clearHits();

  TPaletteAxis* palette = (TPaletteAxis*) histogram()->GetListOfFunctions()->FindObject("palette");

  if (track && track->fitGood()) {
    double z_min = histogram()->GetYaxis()->GetXmin();
    double z_max = histogram()->GetYaxis()->GetXmax();

    double y0 = 0., slopeY = 0.;
    if (track->type() == Track::StraightLine) {
      const StraightLine* straightLine = static_cast<const StraightLine*>(track);
      double x0 = straightLine->x0();
      double slopeX = straightLine->slopeX();
      double x_min = x0 + z_min * slopeX;
      double x_max = x0 + z_max * slopeX;

      TLine* x_line = new TLine(x_min, z_min, x_max, z_max);
      x_line->SetLineColor(kBlack);
      x_line->SetLineWidth(1);
      x_line->Draw("SAME");
      m_lines.push_back(x_line);

      // strech because we want to show x and y in the same view (convert 40cm to 20cm)
      y0 = straightLine->y0();
      slopeY = straightLine->slopeY();
    }
    else if (track->type() == Track::BrokenLine || track->type() == Track::CenteredBrokenLine ) {
      double x0, slopeX, zIntersection, x_min, x_max;
      TLine* x_line;

      // lower line
      if (track->type() == Track::BrokenLine) {
        const BrokenLine* brokenLine = static_cast<const BrokenLine*>(track);
        zIntersection = brokenLine->zIntersection();
        x0 = brokenLine->lowerX0();
        slopeX = brokenLine->lowerSlopeX();
        y0 = brokenLine->y0();
        slopeY = brokenLine->slopeY();
      }
      else if (track->type() == Track::CenteredBrokenLine) {
        const CenteredBrokenLine* centeredBrokenLine = static_cast<const CenteredBrokenLine*>(track);
        zIntersection = centeredBrokenLine->zIntersection();
        x0 = centeredBrokenLine->x0();
        slopeX = centeredBrokenLine->lowerSlopeX();
        y0 = centeredBrokenLine->y0();
        slopeY = centeredBrokenLine->slopeY();
      }
      x_min = x0 + z_min * slopeX;
      x_max = x0 + zIntersection * slopeX;
      x_line = new TLine(x_min, z_min, x_max, zIntersection);
      x_line->SetLineColor(kBlack);
      x_line->SetLineWidth(1);
      x_line->Draw("SAME");
      m_lines.push_back(x_line);

      // upper line
      if (track->type() == Track::BrokenLine) {
        const BrokenLine* brokenLine = static_cast<const BrokenLine*>(track);
        zIntersection = brokenLine->zIntersection();
        x0 = brokenLine->upperX0();
        slopeX = brokenLine->upperSlopeX();
      }
      else if (track->type() == Track::CenteredBrokenLine) {
        const CenteredBrokenLine* centeredBrokenLine = static_cast<const CenteredBrokenLine*>(track);
        zIntersection = centeredBrokenLine->zIntersection();
        x0 = centeredBrokenLine->x0();
        slopeX = centeredBrokenLine->upperSlopeX();
      }
      x_min = x0 + zIntersection * slopeX;
      x_max = x0 + z_max * slopeX;
      x_line = new TLine(x_min, zIntersection, x_max, z_max);
      x_line->SetLineColor(kBlack);
      x_line->SetLineWidth(1);
      x_line->Draw("SAME");
      m_lines.push_back(x_line);
    }

    double y_min = y0 + z_min * slopeY;
    double y_max = y0 + z_max * slopeY;
    TLine* y_line = new TLine(m_yStretchFactor*y_min, z_min, m_yStretchFactor*y_max, z_max);
    y_line->SetLineColor(kRed);
    y_line->SetLineWidth(1);
    y_line->SetLineStyle(1);
    y_line->Draw("SAME");
    m_lines.push_back(y_line);

    char text[128];
    sprintf(text, "#chi^{2} / ndf = %.1f / %d", track->chi2(), track->ndf());
    m_fitInfo = new TLatex(90, z_min + 0.64*(z_max-z_min), text);
    m_fitInfo->SetTextSize(0.03);
    m_fitInfo->Draw("SAME");
  }

  QVector<Hit*> hitsToPlot;

  foreach(Hit* hit, track && !m_drawAllClusters ? track->hits() : hits) {
    // draw TOF yEstimates
    if (strcmp(hit->ClassName(), "TOFCluster") == 0) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(hit);
      TMarker* marker = new TMarker(m_yStretchFactor * tofCluster->yEstimate(), tofCluster->position().z(), 20);
      marker->SetMarkerSize(.5);
      marker->Draw("SAME");
      m_markers.push_back(marker);
    }

    // draw the raw the the rest
    if ( (strcmp(hit->ClassName(), "Hit") == 0) || (strcmp(hit->ClassName(), "TOFSipmHit") == 0) ) {
      hitsToPlot.push_back(hit);
    }
    else if ( (strcmp(hit->ClassName(), "Cluster") == 0) || (strcmp(hit->ClassName(), "TOFCluster") == 0) ) {
      Cluster* cluster = static_cast<Cluster*>(hit);
      foreach(Hit* subHit, cluster->hits())
        hitsToPlot.push_back(subHit);
    }
  }

  foreach(Hit* hit, hitsToPlot) {
    double angle = hit->angle();
    TVector3 position = hit->position();
    TVector3 counterPos = hit->counterPosition();

    if (track) {
      TVector3 trackPos = track->position(position.z());
      trackPos.RotateZ(-angle);
      double u = trackPos.y();

      TVector3 direction = (position - counterPos);
      if (direction.y() < 0)
        direction = -direction;
      direction *= 1./direction.Mag();

      TVector3 calculatedPos = 0.5*(position+counterPos) + u*direction;
      position = calculatedPos;
    }

    int amplitude = hit->signalHeight();
    Hit::ModuleType type = hit->type();
    double x = position.x();
    double z = position.z();
    int color = palette->GetValueColor(amplitude);

    double width = 0.;
    double height = 0.;
    double heightModule = 20.0;
    if (type == Hit::tracker) {
      width = 1.0;
      height = heightModule;
    }
    else if (type == Hit::trd) {
      width = 6.0;
      height = 0.5*heightModule;
    }
    else if (type == Hit::tof) {
      width = 5.;
      height = 6.;
      unsigned short channel = hit->channel();
      DetectorElement* element = Setup::instance()->element(hit->detId() - channel);
      x = element->position().x() + (2*(channel-2)+0.5) * width;
      color = palette->GetValueColor(amplitude*5);
    }

    TBox* box = new TBox(x-0.5*width, z-0.5*height, x+0.5*width, z+0.5*height);
    unsigned int fillstyle = 1001;
    if (track) {
      bool found = false;
      int i = 0;
      while (!found && i < track->hits().size()) {
        Hit* trackHit = track->hits().at(i);
        if ( (strcmp(trackHit->ClassName(), "Hit") == 0) || (strcmp(trackHit->ClassName(), "TOFSipmHit") == 0) ) {
          if (hit == trackHit) {
            found = true;
          }
        }
        else if ( (strcmp(trackHit->ClassName(), "Cluster") == 0) || (strcmp(trackHit->ClassName(), "TOFCluster") == 0) ) {
          Cluster* cluster = static_cast<Cluster*>(trackHit);
          if (std::find(cluster->hits().begin(), cluster->hits().end(), hit) != cluster->hits().end())
            found = true;
        }
        i++;
      }
      fillstyle = found ? 1001 : 3001;
    }
    box->SetFillStyle(fillstyle);
    box->SetFillColor(color);
    box->Draw("SAME");
    m_hits.push_back(box);
  }

  if( event->contentType() == SimpleEvent::MonteCarlo){

    qDeleteAll(m_trajectoriesXZ);
    m_trajectoriesXZ.clear();

    qDeleteAll(m_trajectoriesYZ);
    m_trajectoriesYZ.clear();


    //read all particles:
    const MCEventInformation* mcInfo = event->MCInformation();

    QList<const MCSimpleEventParticle*> allMCParticles;
    //qDebug("draw traj: add primary");
    allMCParticles << mcInfo->primary();
    //qDebug("draw traj: added primary");
    //qDebug("draw traj: adding %i secondaries",  mcInfo->secondaries().size());
    for (unsigned int i = 0; i < mcInfo->secondaries().size(); ++i)
    {
      //qDebug("draw traj: add a secondary");
      allMCParticles << mcInfo->secondaries().at(i) ;
      //qDebug("draw traj: added a secondary");
    }

    //now draw all trajectories
    for (int i = 0; i < allMCParticles.size(); ++i)
    {
      const MCSimpleEventParticle* particle = allMCParticles.at(i);
      const std::vector<TVector3>& trajectory = particle->trajectory;
      QVector<double> x,y,z;
      for (unsigned int j = 0; j < trajectory.size(); ++j){
        double zCoord = trajectory.at(j).Z();
        //if (zCoord < 300 && zCoord > -600) {
          x << trajectory.at(j).X();
          y << 0.5*trajectory.at(j).Y();
          z << zCoord;
        //}
      }

      TGraph* traj = new TGraph(x.size(),&*x.begin(), &*z.begin());
      traj->SetLineColor(8);
      traj->SetLineStyle(5);
      traj->Draw("same L");
      m_trajectoriesXZ.push_back(traj);

      traj = new TGraph(y.size(),&*y.begin(), &*z.begin());
      traj->SetLineColor(46);
      traj->SetLineStyle(5);
      traj->Draw("same L");
      m_trajectoriesYZ.push_back(traj);
    }

  }

}
  
void HitsPlot::setDrawAllClusters(bool value)
{
  m_drawAllClusters = value;
}

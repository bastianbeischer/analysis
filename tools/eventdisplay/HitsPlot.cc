#include "HitsPlot.hh"

#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "Track.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"
#include "StraightLine.hh"
#include "SimpleEvent.hh"
#include "TOFCluster.hh"
#include "DetectorElement.hh"
#include "Setup.hh"

#include <TCanvas.h>
#include <TList.h>
#include <TLine.h>
#include <TVector3.h>
#include <TPolyLine.h>
#include <TLatex.h>
#include <TH2D.h>
#include <TBox.h>
#include <THistPainter.h>
#include <TPaletteAxis.h>
#include <TGaxis.h>
#include <TMarker.h>
#include <TStyle.h>

#include <iostream>

HitsPlot::HitsPlot()
  : PerdaixDisplay()
  , EventDestination()
  , m_fitInfo(0)
  , m_trajectoryXZ(new TPolyLine())
  , m_trajectoryYZ(new TPolyLine())
{
  m_trajectoryXZ->SetLineColor(8);
  m_trajectoryYZ->SetLineColor(46);
  m_trajectoryXZ->SetLineStyle(5);;
  m_trajectoryYZ->SetLineStyle(5);
}

HitsPlot::~HitsPlot()
{
  clearHits();
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

void HitsPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent* event)
{
  clearHits();

  TPaletteAxis* palette = (TPaletteAxis*) histogram()->GetListOfFunctions()->FindObject("palette");

  if (track && track->fitGood()) {
    double z_min = histogram()->GetYaxis()->GetXmin();
    double z_max = histogram()->GetYaxis()->GetXmax();

    double y0 = 0., slopeY = 0.;
    if (track->type() == Track::StraightLine) {
      StraightLine* straightLine = static_cast<StraightLine*>(track);
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
        BrokenLine* brokenLine = static_cast<BrokenLine*>(track);
        zIntersection = brokenLine->zIntersection();
        x0 = brokenLine->lowerX0();
        slopeX = brokenLine->lowerSlopeX();
        y0 = brokenLine->y0();
        slopeY = brokenLine->slopeY();
      }
      else if (track->type() == Track::CenteredBrokenLine) {
        CenteredBrokenLine* centeredBrokenLine = static_cast<CenteredBrokenLine*>(track);
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
        BrokenLine* brokenLine = static_cast<BrokenLine*>(track);
        zIntersection = brokenLine->zIntersection();
        x0 = brokenLine->upperX0();
        slopeX = brokenLine->upperSlopeX();
      }
      else if (track->type() == Track::CenteredBrokenLine) {
        CenteredBrokenLine* centeredBrokenLine = static_cast<CenteredBrokenLine*>(track);
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

  foreach(Hit* hit, hits) {
    // draw TOF yEstimates
    if (strcmp(hit->ClassName(), "TOFCluster") == 0) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(hit);
      if (tofCluster->signalHeight() > 4*200) {
        TMarker* marker = new TMarker(m_yStretchFactor * tofCluster->yEstimate(), tofCluster->position().z(), 20);
        marker->SetMarkerSize(.5);
        marker->Draw("SAME");
        m_markers.push_back(marker);
      }
    }
    /*
      TLine* y_tofErrorLine = new TLine(
      m_yStretchFactor * (tofCluster->yEstimate() - tofCluster->yResolutionEstimate()),
      tofCluster->position().z(),
      m_yStretchFactor * (tofCluster->yEstimate() + tofCluster->yResolutionEstimate()),
      tofCluster->position().z()
      );
      y_tofErrorLine->SetLineColor(kRed);
      y_tofErrorLine->SetLineWidth(1);
      y_tofErrorLine->SetLineStyle(1);
      y_tofErrorLine->Draw("SAME");
      m_lines.push_back(y_tofErrorLine); */

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
    box->SetFillStyle(1001);
    box->SetFillColor(color);
    box->Draw("SAME");
    m_hits.push_back(box);
  }

  if( event->contentType() == 3){
    const MCEventInformation* mcInfo = event->MCInformation();
    std::vector<TVector3> trajectory = mcInfo->trajectory();
    QVector<double> x,y,z;
    for (unsigned int i = 0; i < trajectory.size(); ++i){
      double zCoord = trajectory.at(i).Z();
      //if (zCoord < 300 && zCoord > -600) {
        x << trajectory.at(i).X();
        y << 0.5*trajectory.at(i).Y();
        z << zCoord;
      //}
    }


    m_trajectoryXZ->SetPolyLine(x.size(),&*x.begin(), &*z.begin());
    m_trajectoryXZ->Draw("same");

    m_trajectoryYZ->SetPolyLine(y.size(),&*y.begin(), &*z.begin());
    m_trajectoryYZ->Draw("same");
  }

}

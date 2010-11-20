#include "HitsPlot.hh"

#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "Track.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"
#include "StraightLine.hh"
#include "TOFCluster.hh"

#include <TCanvas.h>
#include <TList.h>
#include <TLine.h>
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
  , m_fitInfo(0)
{
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

void HitsPlot::drawEvent(TCanvas* canvas, const QVector<Hit*>& hits, Track* track)
{
  canvas->cd();
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

  foreach(Hit* hit, hits) {
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

    if (strcmp(hit->ClassName(), "TOFCluster") == 0) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(hit);
      if (tofCluster->signalHeight() > 4*200) {
        TMarker* marker = new TMarker(m_yStretchFactor * tofCluster->yEstimate(), tofCluster->position().z(), 20);
        marker->SetMarkerSize(.5);
        marker->Draw("SAME");
        m_markers.push_back(marker);
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
      width = 6.0;
      height = 3.0;
    }

    TBox* box = new TBox(x-0.5*width, z-0.5*height, x+0.5*width, z+0.5*height);
    //    std::cout << "adding box at " << position.z()  << std::endl;
    box->SetFillStyle(1001);
    box->SetFillColor(color);
    box->Draw("SAME");
    m_hits.push_back(box);
  }

  canvas->Modified();
  canvas->Update();
}

#include "HitsPlot.hh"

#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "Track.hh"
#include "BrokenLine.hh"
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
  : m_positionHist(new TH2D("2D EventDisplay", "", 100, -200., 200., 100, -650, 350))
  , m_yAxis(0)
  , m_fitInfo(0)
{
  m_positionHist->GetXaxis()->SetTitle("x / mm");
  m_positionHist->GetYaxis()->SetTitle("z / mm");
  m_positionHist->GetZaxis()->SetTitle("signal height / ADC counts");
  m_positionHist->GetZaxis()->SetTitleOffset(1.4);
  m_positionHist->SetContour(99);
  m_positionHist->SetMinimum(0);
  m_positionHist->SetMaximum(4096);
  m_positionHist->Fill(1e5, 1e5);
  m_positionHist->SetStats(false);
  m_positionHist->Draw("colz");
  gPad->Update();
  const double widthModule  = 65.;
  const double heightModule = 20.;
  const double zTracker[4] = {227., 60., -60., -227};

  const double trdWidth  = 200.;
  const double trdHeight = 222.;
  const double trdX0     = 0.;
  const double trdZ0     = -411.;

  const double tofHeight = 6.;
  const double tofWidth  = 50.;
  const double zUpperLayer = 406.55 - 125.;
  const double zLowerLayer = -406.55 - 125.;
  const double zTof[4]   = {zUpperLayer + 0.5*tofHeight,
                            zUpperLayer - 0.5*tofHeight,
                            zLowerLayer + 0.5*tofHeight, 
                            zLowerLayer - 0.5*tofHeight};

  m_boxes.push_back(new TBox(-1.5*widthModule, zTracker[0] - heightModule/2., -0.5*widthModule, zTracker[0] + heightModule/2.));
  m_boxes.push_back(new TBox(-0.5*widthModule, zTracker[0] - heightModule/2.,  0.5*widthModule, zTracker[0] + heightModule/2.));
  m_boxes.push_back(new TBox( 0.5*widthModule, zTracker[0] - heightModule/2.,  1.5*widthModule, zTracker[0] + heightModule/2.));
    
  m_boxes.push_back(new TBox(-1.0*widthModule, zTracker[1] - heightModule/2.,  0.0*widthModule, zTracker[1] + heightModule/2.));
  m_boxes.push_back(new TBox( 0.0*widthModule, zTracker[1] - heightModule/2.,  1.0*widthModule, zTracker[1] + heightModule/2.));

  m_boxes.push_back(new TBox(-1.0*widthModule, zTracker[2] - heightModule/2.,  0.0*widthModule, zTracker[2] + heightModule/2.));
  m_boxes.push_back(new TBox( 0.0*widthModule, zTracker[2] - heightModule/2.,  1.0*widthModule, zTracker[2] + heightModule/2.));

  m_boxes.push_back(new TBox(-1.5*widthModule, zTracker[3] - heightModule/2., -0.5*widthModule, zTracker[3] + heightModule/2.));
  m_boxes.push_back(new TBox(-0.5*widthModule, zTracker[3] - heightModule/2.,  0.5*widthModule, zTracker[3] + heightModule/2.));
  m_boxes.push_back(new TBox( 0.5*widthModule, zTracker[3] - heightModule/2.,  1.5*widthModule, zTracker[3] + heightModule/2.));
     
  m_boxes.push_back(new TBox(trdX0 - 0.5*trdWidth, trdZ0 - 0.5*trdHeight, trdX0 + 0.5*trdWidth, trdZ0 + 0.5*trdHeight));

  m_boxes.push_back(new TBox(-2.0*tofWidth, zTof[0] - 0.5*tofHeight, -1.0*tofWidth, zTof[0] + 0.5*tofHeight));
  m_boxes.push_back(new TBox(-1.0*tofWidth, zTof[0] - 0.5*tofHeight, -0.0*tofWidth, zTof[0] + 0.5*tofHeight));
  m_boxes.push_back(new TBox( 0.0*tofWidth, zTof[0] - 0.5*tofHeight,  1.0*tofWidth, zTof[0] + 0.5*tofHeight));
  m_boxes.push_back(new TBox( 1.0*tofWidth, zTof[0] - 0.5*tofHeight,  2.0*tofWidth, zTof[0] + 0.5*tofHeight));

  m_boxes.push_back(new TBox(-2.0*tofWidth, zTof[1] - 0.5*tofHeight, -1.0*tofWidth, zTof[1] + 0.5*tofHeight));
  m_boxes.push_back(new TBox(-1.0*tofWidth, zTof[1] - 0.5*tofHeight, -0.0*tofWidth, zTof[1] + 0.5*tofHeight));
  m_boxes.push_back(new TBox( 0.0*tofWidth, zTof[1] - 0.5*tofHeight,  1.0*tofWidth, zTof[1] + 0.5*tofHeight));
  m_boxes.push_back(new TBox( 1.0*tofWidth, zTof[1] - 0.5*tofHeight,  2.0*tofWidth, zTof[1] + 0.5*tofHeight));

  m_boxes.push_back(new TBox(-2.0*tofWidth, zTof[2] - 0.5*tofHeight, -1.0*tofWidth, zTof[2] + 0.5*tofHeight));
  m_boxes.push_back(new TBox(-1.0*tofWidth, zTof[2] - 0.5*tofHeight, -0.0*tofWidth, zTof[2] + 0.5*tofHeight));
  m_boxes.push_back(new TBox( 0.0*tofWidth, zTof[2] - 0.5*tofHeight,  1.0*tofWidth, zTof[2] + 0.5*tofHeight));
  m_boxes.push_back(new TBox( 1.0*tofWidth, zTof[2] - 0.5*tofHeight,  2.0*tofWidth, zTof[2] + 0.5*tofHeight));

  m_boxes.push_back(new TBox(-2.0*tofWidth, zTof[3] - 0.5*tofHeight, -1.0*tofWidth, zTof[3] + 0.5*tofHeight));
  m_boxes.push_back(new TBox(-1.0*tofWidth, zTof[3] - 0.5*tofHeight, -0.0*tofWidth, zTof[3] + 0.5*tofHeight));
  m_boxes.push_back(new TBox( 0.0*tofWidth, zTof[3] - 0.5*tofHeight,  1.0*tofWidth, zTof[3] + 0.5*tofHeight));
  m_boxes.push_back(new TBox( 1.0*tofWidth, zTof[3] - 0.5*tofHeight,  2.0*tofWidth, zTof[3] + 0.5*tofHeight));
  
  foreach(TBox* box, m_boxes) {
    box->SetFillStyle(0);
    box->SetLineStyle(1);
    box->Draw("SAME");
  }

  // new axis for y
  double min = -400;
  double max = 400;
  m_yAxis = new TGaxis(gPad->GetUxmin(), gPad->GetUymax(), gPad->GetUxmax(), gPad->GetUymax(), min, max, 510, "-");
  m_yAxis->SetLineColor(kRed);
  m_yAxis->SetTextColor(kRed);
  m_yAxis->SetLabelColor(kRed);
  m_yAxis->SetLabelOffset(0);
  m_yAxis->SetTitle("y / mm");
  m_yAxis->SetTitleOffset(1.2);
  m_yAxis->SetLabelFont(gStyle->GetLabelFont());
  m_yAxis->SetLabelSize(gStyle->GetLabelSize());
  m_yAxis->SetTitleFont(gStyle->GetTitleFont());
  m_yAxis->SetTitleSize(gStyle->GetTitleSize());
  m_yAxis->Draw();
  gPad->Update();
  m_stretchFactor =
    (m_positionHist->GetXaxis()->GetXmax() - m_positionHist->GetXaxis()->GetXmin())
    / (m_yAxis->GetWmax() - m_yAxis->GetWmin());
}

HitsPlot::~HitsPlot()
{
  delete m_positionHist;
  delete m_yAxis;
  qDeleteAll(m_boxes);
  clear();
}

void HitsPlot::clear()
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

double HitsPlot::yStretchFactor()
{
  return m_stretchFactor;
}

void HitsPlot::draw(TCanvas* canvas, QVector<Hit*> hits, Track* track)
{
  canvas->cd();
  clear();

  TPaletteAxis* palette = (TPaletteAxis*) m_positionHist->GetListOfFunctions()->FindObject("palette");


  if (track) {
    double z_min = m_positionHist->GetYaxis()->GetXmin();
    double z_max = m_positionHist->GetYaxis()->GetXmax();

    StraightLine* straightLine = dynamic_cast<StraightLine*>(track);
    if (straightLine) {
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
      double y0 = straightLine->y0();
      double slopeY = straightLine->slopeY();
      double y_min = y0 + z_min * slopeY;
      double y_max = y0 + z_max * slopeY;

      TLine* y_line = new TLine(m_stretchFactor*y_min, z_min, m_stretchFactor*y_max, z_max);
      y_line->SetLineColor(kRed);
      y_line->SetLineWidth(1);
      y_line->SetLineStyle(1);
      y_line->Draw("SAME");
      m_lines.push_back(y_line);
    }

    BrokenLine* brokenLine = dynamic_cast<BrokenLine*>(track);
    if (brokenLine) {
      double zIntersection = brokenLine->zIntersection();

      double x0, slopeX, x_min, x_max;
      TLine* x_line;

      // lower line
      x0 = brokenLine->lowerX0();
      slopeX = brokenLine->lowerSlopeX();
      x_min = x0 + z_min * slopeX;
      x_max = x0 + zIntersection * slopeX;
      x_line = new TLine(x_min, z_min, x_max, zIntersection);
      x_line->SetLineColor(kBlack);
      x_line->SetLineWidth(2);
      x_line->Draw("SAME");
      m_lines.push_back(x_line);

      // upper line
      x0 = brokenLine->upperX0();
      slopeX = brokenLine->upperSlopeX();
      x_min = x0 + zIntersection * slopeX;
      x_max = x0 + z_max * slopeX;
      x_line = new TLine(x_min, zIntersection, x_max, z_max);
      x_line->SetLineColor(kBlack);
      x_line->SetLineWidth(2);
      x_line->Draw("SAME");
      m_lines.push_back(x_line);

      // strech because we want to show x and y in the same view (convert 40cm to 20cm)
      double y0 = brokenLine->y0();
      double slopeY = brokenLine->slopeY();
      double y_min = y0 + z_min * slopeY;
      double y_max = y0 + z_max * slopeY;

      TLine* y_line = new TLine(m_stretchFactor*y_min, z_min, m_stretchFactor*y_max, z_max);
      y_line->SetLineColor(kRed);
      y_line->SetLineWidth(1);
      y_line->SetLineStyle(1);
      y_line->Draw("SAME");
      m_lines.push_back(y_line);
    }
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

    TOFCluster* tofCluster = dynamic_cast<TOFCluster*>(hit);
    if (tofCluster) {
      if (tofCluster->signalHeight() > 4*200) {
        TMarker* marker = new TMarker(m_stretchFactor * tofCluster->yEstimate(), tofCluster->position().z(), 20);
        marker->SetMarkerSize(.5);
        marker->Draw("SAME");
        m_markers.push_back(marker);
      }

      /*
        TLine* y_tofErrorLine = new TLine(
        m_stretchFactor * (tofCluster->yEstimate() - tofCluster->yResolutionEstimate()),
        tofCluster->position().z(),
        m_stretchFactor * (tofCluster->yEstimate() + tofCluster->yResolutionEstimate()),
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

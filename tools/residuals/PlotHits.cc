#include "PlotHits.hh"

#include "Hit.hh"
#include "Track.hh"

#include <TCanvas.h>
#include <TList.h>
#include <TLine.h>
#include <TLatex.h>
#include <TH2D.h>
#include <TBox.h>
#include <THistPainter.h>
#include <TPaletteAxis.h>

#include <iostream>

unsigned int PlotHits::saves = 0;

PlotHits::PlotHits() :
  m_canvas(new TCanvas("PlotHits", "PlotHits", 1300, 900)),
  m_positionHist(new TH2D("2D EventDisplay", "2D Event Display", 100, -200., 200., 100, -650, 350)),
  m_line(0),
  m_fitInfo(0)
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

  m_canvas->SetRightMargin(0.16);
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
}

PlotHits::~PlotHits()
{
  delete m_canvas;
  delete m_positionHist;
  foreach(TBox* box, m_boxes)
    delete box;
  clear();
}

void PlotHits::clear()
{
  foreach(TBox* hitBox, m_hits)
    delete hitBox;
  m_hits.clear();
  delete m_line;
  m_line = 0;
  delete m_fitInfo;
  m_fitInfo = 0;
}

void PlotHits::plot(QVector<Hit*> hits, Track* track)
{
  m_canvas->cd();
  clear();

  TPaletteAxis* palette = (TPaletteAxis*) m_positionHist->GetListOfFunctions()->FindObject("palette");

  foreach(Hit* hit, hits) {
    TVector3 position = hit->position();
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

    TBox* box = new TBox(x-0.5*width, z-0.5*height, x+0.5*width, z+0.5*height);
    //    std::cout << "adding box at " << position.z()  << std::endl;
    box->SetFillStyle(1001);
    box->SetFillColor(color);
    box->Draw("SAME");
    m_hits.push_back(box);
  }

  if (track) {
    double x0 = track->x0();
    double slopeX = track->slopeX();
    double z_min = m_positionHist->GetYaxis()->GetXmin();
    double z_max = m_positionHist->GetYaxis()->GetXmax();
    double x_min = x0 + z_min * slopeX;
    double x_max = x0 + z_max * slopeX;

    m_line = new TLine(x_min, z_min, x_max, z_max);
    m_line->SetLineColor(kBlack);
    m_line->SetLineWidth(2);
    m_line->Draw("SAME");

    char text[128];
    sprintf(text, "#chi^{2} / ndf = %.1f / %d", track->chi2(), track->ndf());
    m_fitInfo = new TLatex(-50, z_min + 1.05*(z_max-z_min), text);
    m_fitInfo->Draw("SAME");
  }

  m_canvas->Modified();

  saveCanvas("png");
}

void PlotHits::saveCanvas(const char* format)
{
  const char* title = m_canvas->GetTitle();
  char fileName[128];
  sprintf(fileName, "%s_%03d.%s", title, saves, format);
  m_canvas->SaveAs(fileName);
  saves++;
}

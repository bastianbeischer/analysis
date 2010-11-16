#include "PerdaixDisplay.hh"

#include <TH2D.h>
#include <TBox.h>
#include <TPad.h>
#include <TGaxis.h>
#include <TStyle.h>

PerdaixDisplay::PerdaixDisplay() :
  H2DPlot()
{
  TH2D* histogram = new TH2D("PerdaixDisplay", "", 100, -200., 200., 100, -650, 350);
  histogram->GetXaxis()->SetTitle("x / mm");
  histogram->GetYaxis()->SetTitle("z / mm");
  histogram->GetZaxis()->SetTitle("signal height / ADC counts");
  histogram->GetZaxis()->SetTitleOffset(1.4);
  histogram->SetContour(99);
  histogram->SetMinimum(0);
  histogram->SetMaximum(4096);
  histogram->Fill(1e5, 1e5);
  histogram->SetStats(false);
  histogram->Draw("colz");
  setHistogram(histogram);
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
  m_yStretchFactor = (histogram->GetXaxis()->GetXmax() - histogram->GetXaxis()->GetXmin()) / (m_yAxis->GetWmax() - m_yAxis->GetWmin());
}

PerdaixDisplay::~PerdaixDisplay()
{
  delete m_yAxis;
  qDeleteAll(m_boxes);
}


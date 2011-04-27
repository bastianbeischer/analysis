#include "PerdaixDisplay.hh"

#include <TCanvas.h>
#include <TH2D.h>
#include <TBox.h>
#include <TPad.h>
#include <TGaxis.h>
#include <TStyle.h>

PerdaixDisplay::PerdaixDisplay() :
  H2DPlot(),
  m_yStretchFactor(0.),
  m_yAxis(new TGaxis(-200, 350, 200, 350, -400, 400, 510, "-"))
{
  TH2D* histogram = new TH2D("PerdaixDisplay", "", 100, -200., 200., 100, -650, 350);
  addHistogram(histogram);
  constructTracker();
  constructTrd();
  constructTof();
  setupHistogram();
  setupAxis();
}

PerdaixDisplay::~PerdaixDisplay()
{
  delete m_yAxis;
  qDeleteAll(m_boxes);
}

void PerdaixDisplay::draw(TCanvas* can)
{
  H2DPlot::draw(can);
  foreach(TBox* box, m_boxes) {
    box->SetFillStyle(0);
    box->SetLineStyle(1);
    box->Draw("SAME");
  }
  m_yAxis->Draw();
}

void PerdaixDisplay::constructTracker()
{
  const double widthModule  = 65.;
  const double heightModule = 20.;
  const double zTracker[4] = {227., 60., -60., -227};
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
}

void PerdaixDisplay::constructTrd()
{
  const double trdWidth  = 200.;
  const double trdHeight = 222.;
  const double trdX0     = 0.;
  const double trdZ0     = -411.;
  m_boxes.push_back(new TBox(trdX0 - 0.5*trdWidth, trdZ0 - 0.5*trdHeight, trdX0 + 0.5*trdWidth, trdZ0 + 0.5*trdHeight));
}

void PerdaixDisplay::constructTof()
{
  const double tofHeight = 6.;
  const double tofWidth  = 50.;
  const double zUpperLayer = 406.55 - 125.;
  const double zLowerLayer = -406.55 - 125.;
  const double zTof[4]   = {zUpperLayer + 0.5*tofHeight, zUpperLayer - 0.5*tofHeight, zLowerLayer + 0.5*tofHeight, zLowerLayer - 0.5*tofHeight};

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
}

void PerdaixDisplay::setupHistogram()
{
  TH2D* h = histogram();
  if (h) {
    h->GetXaxis()->SetTitle("x / mm");
    h->GetYaxis()->SetTitle("z / mm");
    h->GetZaxis()->SetTitle("signal height / ADC counts");
    h->GetZaxis()->SetTitleOffset(1.4);
    h->SetContour(99);
    h->SetMinimum(0);
    h->SetMaximum(4096);
    h->Fill(1e5, 1e5);
    h->SetStats(false);
  }
}

void PerdaixDisplay::setupAxis()
{
  if (m_yAxis) {
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
    TH2D* h = histogram();
    if (h) m_yStretchFactor = (h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin()) / (m_yAxis->GetWmax() - m_yAxis->GetWmin());
  }
}

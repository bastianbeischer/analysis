#include "ResidualPlot.hh"

#include "Hit.hh"
#include "Track.hh"

#include <TCanvas.h>
#include <TH2I.h>
#include <TH1I.h>
#include <TVector3.h>

ResidualPlot::ResidualPlot() :
  m_canvas1D(0),
  m_canvas2D(0),
  m_residuals2D(0)
{
}

ResidualPlot::ResidualPlot(double z) :
  m_z(z),
  m_canvas1D(0),
  m_canvas2D(0),
  m_residuals2D(0)
{
  char title[128];
  sprintf(title, "layer_%.2f", m_z);
  m_residuals2D = new TH2I(title, title, 100, -100, 100, 300, -10, 10);
}

ResidualPlot::~ResidualPlot()
{
  delete m_canvas1D;
  delete m_canvas2D;
  for (std::map<unsigned short, TH1I*>::iterator it = m_residuals1D.begin(); it != m_residuals1D.end(); it++)
    delete it->second;
  delete m_residuals2D;
}

void ResidualPlot::fill(Hit* hit, Track* track)
{
  TVector3 pos = 0.5* (hit->position() + hit->counterPosition());
  TVector3 trackPos = track->position(m_z);

  double angle = hit->angle();
  pos.RotateZ(-angle);
  trackPos.RotateZ(-angle);

  double res = (pos - trackPos).x();

  unsigned short detId = hit->detId();
  detId -= hit->channel();
  if (!m_residuals1D[detId]) {
    char title[128];
    sprintf(title, "%x", detId);
    m_residuals1D[detId] = new TH1I(title, title, 300, -10, 10);
  }

  m_residuals1D[detId]->Fill(res);
  m_residuals2D->Fill(track->x(m_z), res);

}

void ResidualPlot::draw()
{
  if (!m_canvas1D) {
    char title[128];
    sprintf(title, "layer_%.0f_1D", m_z);
    m_canvas1D = new TCanvas(title, title);
  }

  m_canvas1D->Divide(6,4);
  int i = 1;
  for (std::map<unsigned short, TH1I*>::iterator it = m_residuals1D.begin(); it != m_residuals1D.end(); it++) {
    m_canvas1D->cd(i);
    TH1I* hist = it->second;
    hist->Draw();
    i++;
  }

  if (!m_canvas2D) {
    char title[128];
    sprintf(title, "layer_%.0f_2D", m_z);
    m_canvas2D = new TCanvas(title, title);
  }

  m_canvas2D->Draw();
  m_canvas2D->Divide(1,2);
  m_canvas2D->cd(1);
  m_residuals2D->Draw("colz");
  m_canvas2D->cd(2);
  TH1D* projection = m_residuals2D->ProjectionY();
  projection->Draw();

  // m_canvas1D->SaveAs(".pdf");
  // m_canvas1D->SaveAs(".png");
  // m_canvas1D->SaveAs(".root");

  // m_canvas2D->SaveAs(".pdf");
  // m_canvas2D->SaveAs(".png");
  // m_canvas2D->SaveAs(".root");
}

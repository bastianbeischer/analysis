#include "RootStyle.hh"

#include <DataChain.hh>
#include <SimpleEvent.hh>

#include <TApplication.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TSpectrum.h>
#include <TF1.h>
#include <TGraph.h>

#include <iostream>
#include <cmath>

#include <QDebug>

int nPeaks = 0;

double gauss(double x, double c, double mu, double sigma)
{
  return c * exp(- (x-mu)*(x-mu) / (sigma*sigma));
}

double polyGauss(double x[], double p[])
{
  double ret = 0;
  for (int i = 0; i < nPeaks; ++i)
    ret += gauss(x[0], p[3*i+0], p[3*i+1], p[3*i+2]);
  return ret;
}

int main(int argc, char* argv[])
{
  RootStyle::set();
  DataChain* chain = new DataChain;
  chain->addFileList(argv[1]);
  int entries = chain->nEntries();
  unsigned int previousTime = 0;
  TH1D* h = new TH1D("dt", ";#Deltat / ms;", 100, 0, 100);
  for (int it = 0; it < entries; ++it) {
    SimpleEvent* event = chain->event(it);
    if (it == 0) {
      previousTime = event->eventTime();
    } else {
      h->Fill(event->eventTime() - previousTime);
      previousTime = event->eventTime();
      //std::cout << event->eventTime() - previousTime << std::endl;
      if (it % 10000 == 0)
        std::cerr << '.' << std::flush;
      //if (it == 20000) break;
    }
  }
  std::cerr << std::endl;
  //h->Smooth(4);
  TSpectrum spectrum;
  nPeaks = spectrum.Search(h, .5, "", 0.01);
 
  TGraph* g1 = new TGraph;
  for (int i = 0; i < nPeaks; ++i) {
    g1->SetPoint(i, spectrum.GetPositionX()[i], spectrum.GetPositionY()[i]);
  }
  g1->Sort();
  TF1* fg1 = new TF1("exponential", "[0]*exp(-[1]*x)", 0, 100);
  g1->Fit(fg1);

  TF1* f2 = new TF1("polyGauss", polyGauss, 0, 100, 3*nPeaks);
  for (int i = 0; i < nPeaks; ++i) {
    f2->SetParameter(3*i+0, spectrum.GetPositionY()[i]);
    f2->SetParameter(3*i+1, spectrum.GetPositionX()[i]);
    f2->SetParLimits(3*i+1, spectrum.GetPositionX()[i]-1, spectrum.GetPositionX()[i]+1);
    f2->SetParameter(3*i+2, 2);
  }
  h->Fit(f2, "QN0");
  TGraph* g2 = new TGraph;
  for (int i = 0; i < nPeaks; ++i) {
    double c = f2->GetParameter(3*i+0);
    double mu = f2->GetParameter(3*i+1);
    double sigma = f2->GetParameter(3*i+2);
    g2->SetPoint(i, mu, c*sigma*sqrt(2.*M_PI));
  }
  g2->Sort();
  TF1* fg2 = new TF1("exponential", "[0]*exp(-[1]*x)", 0, 100);
  g2->Fit(fg2);

  TApplication application(argv[0], &argc, argv);
  new TCanvas;
  h->Draw();
  new TCanvas();
  g1->Draw("ALP");
  new TCanvas;
  h->Draw();
  f2->SetNpx(1000);
  f2->Draw("SAME");
  new TCanvas;
  g2->Draw("ALP");
  application.Run();
  return 0;
}

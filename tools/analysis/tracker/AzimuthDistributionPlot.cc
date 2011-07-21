#include "AzimuthDistributionPlot.hh"

#include <QDebug>

#include <TH1D.h>
#include <TLatex.h>
#include <TFile.h>
#include <TROOT.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"

#include <iostream>
#include <cmath>

AzimuthDistributionPlot::AzimuthDistributionPlot() :
AnalysisPlot(AnalysisPlot::Tracking),
H1DPlot()
{
  setTitle("Azimuth distribution");
  int nBins = 60;
  double xMin = -180;
  double xMax = 180;
  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, xMin, xMax);
  histogram->Sumw2();
  setAxisTitle("azimuth", "probability / %");
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(.15, .85));
  
  readFile();
  addHistogram(m_azimuthAcceptance, H1DPlot::HIST);
}

AzimuthDistributionPlot::~AzimuthDistributionPlot()
{

}

void AzimuthDistributionPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();
  
  if (!track || !track->fitGood())
    return;
  
  ParticleInformation::Flags flags = particle->information()->flags();
  if ( !(flags & ParticleInformation::AllTrackerLayers) || !(flags & ParticleInformation::InsideMagnet) || (flags & ParticleInformation::Albedo) )
    return;
  
  double azimuth = (track->azimuthAngle()) / M_PI * 180;
  
  histogram()->Fill(azimuth);
}

void AzimuthDistributionPlot::update()
{
  double integral = histogram(0)->Integral("width");
  histogram(0)->Scale(100/integral);
}

void AzimuthDistributionPlot::finalize()
{
  update();
}

void AzimuthDistributionPlot::readFile()
{
  QString filename = "azimuthAcceptance.root";
  std::cout << "Reading file " <<qPrintable(filename) << std::endl;
  QString hName = "phiHistoValid";
  TFile* openfile = new TFile(qPrintable(filename));
  gROOT->cd();
  TH1D* azimuthAcceptance = (TH1D*)(((TH1D*)openfile->Get(qPrintable(hName)))->Clone());
  double integral = azimuthAcceptance->Integral("width");
  azimuthAcceptance->Scale(100/integral);
  openfile->Close();
  delete openfile;
  openfile = 0;
  
  m_azimuthAcceptance = new TH1D("acceptance phi", "", azimuthAcceptance->GetNbinsX(), -180, 180);
  for (int i = 0; i < m_azimuthAcceptance->GetNbinsX(); ++i) {
    double azimuth = m_azimuthAcceptance->GetBinCenter(i+1);
    int bin = azimuthAcceptance->FindBin(angleMovement(azimuth));
    double content = azimuthAcceptance->GetBinContent(bin);
    m_azimuthAcceptance->SetBinContent(i, content);
  }
  delete azimuthAcceptance;
  azimuthAcceptance = 0;
  m_azimuthAcceptance->SetMarkerColor(kBlue);
  m_azimuthAcceptance->SetLineColor(kBlue);
}

double AzimuthDistributionPlot::angleMovement(double azimuth)
{
  double newAzimuth = azimuth + 90;
  if (azimuth < -90)
    newAzimuth += 360;
  return newAzimuth;
}

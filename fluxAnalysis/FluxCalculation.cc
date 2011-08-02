#include "FluxCalculation.hh"
#include "Constants.hh"
#include "RootPlot.hh"
#include "H1DPlot.hh"
#include "Helpers.hh"

#include <QDebug>
#include <QMutex>

#include <TArray.h>

#include <cmath>

FluxCalculation::FluxCalculation()
{
}

FluxCalculation::FluxCalculation(TH1D* particleHistogram, double measurementTime) :
  m_particleHistogram(particleHistogram),
  m_measurementTime(measurementTime)
{
  const QString title = QString(m_particleHistogram->GetTitle()) + "_flux";
  const QString xTitle = m_particleHistogram->GetXaxis()->GetTitle();
  const int nBins = m_particleHistogram->GetNbinsX();
  const double* axis = m_particleHistogram->GetXaxis()->GetXbins()->GetArray();
  
  m_fluxHistogram = new TH1D(qPrintable(title), "", nBins, axis);
  m_fluxHistogram->Sumw2();
  m_fluxHistogram->GetXaxis()->SetTitle(qPrintable(xTitle));
  m_fluxHistogram->GetYaxis()->SetTitle("flux / (GV^{-1}m^{-2}s^{-1}sr^{-1})");
  m_fluxHistogram->SetLineColor(kBlack);
  m_fluxHistogram->SetMarkerColor(kBlack);
  m_fluxHistogram->SetMarkerStyle(20);
  
  update();
}

FluxCalculation::~FluxCalculation()
{
}

void FluxCalculation::setMeasurementTime(double measurementTime)
{
  m_measurementTime = measurementTime;
}

void FluxCalculation::update()
{
  m_mutexUpdate.lock();
  m_fluxHistogram->Reset();
  double acceptance = Constants::geometricAcceptance;
  
  const int nBins = m_particleHistogram->GetNbinsX();
  
  for (int i = 0 ; i < nBins; i++) {
    double content = m_particleHistogram->GetBinContent(i+1);
    double error = m_particleHistogram->GetBinError(i+1);
    m_fluxHistogram->SetBinContent(i+1, content);
    m_fluxHistogram->SetBinError(i+1, error);
  }
  
  m_fluxHistogram->Scale(1./(acceptance * m_measurementTime), "width");
  m_mutexUpdate.unlock();
}

void FluxCalculation::efficiencyCorrection(TH1D* histogramToCorrect, TH1D* efficiencyHistogram)
{
  if (!histogramToCorrect->GetSumw2())
    histogramToCorrect->Sumw2();
  histogramToCorrect->Multiply(efficiencyHistogram);
}

void FluxCalculation::efficiencyCorrection(TH1D* efficiencyHistogram)
{
  efficiencyCorrection(m_fluxHistogram, efficiencyHistogram);
}

TLatex FluxCalculation::binTitle(int bin)
{
  m_mutexBinTitle.lock();
  double x = m_fluxHistogram->GetBinCenter(bin);
  double y = m_fluxHistogram->GetBinContent(bin);
  double value = m_particleHistogram->GetBinContent(bin);
  m_mutexBinTitle.unlock();

  QString text = "#scale[0.6]{"+QString::number(value,'d',0)+"}";

  TLatex latext(x*1.1, y*1.1, qPrintable(text));
  latext.SetTextFont(42);
  latext.SetTextSize(0.02);
  latext.SetLineWidth(2);
  latext.SetTextAngle(45);
  latext.SetTextColor(kBlack);

  return latext;
}

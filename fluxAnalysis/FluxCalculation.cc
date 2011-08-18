#include "FluxCalculation.hh"
#include "Constants.hh"

#include <QDebug>

#include <TArray.h>
#include <TH1.h>

FluxCalculation::FluxCalculation()
{
}

FluxCalculation::FluxCalculation(TH1D* particleHistogram, double measurementTime)
  : m_particleHistogram(particleHistogram)
{
  const QString& title = QString(m_particleHistogram->GetTitle()) + "_flux";
  const QString& xTitle = m_particleHistogram->GetXaxis()->GetTitle();
  const int nBins = m_particleHistogram->GetNbinsX();
  const double* axis = m_particleHistogram->GetXaxis()->GetXbins()->GetArray();

  m_fluxHistogram = new TH1D(qPrintable(title), "", nBins, axis);
  m_fluxHistogram->Sumw2();
  m_fluxHistogram->GetXaxis()->SetTitle(qPrintable(xTitle));
  m_fluxHistogram->GetYaxis()->SetTitle("flux / (GV^{-1}m^{-2}s^{-1}sr^{-1})");
  m_fluxHistogram->SetLineColor(kBlack);
  m_fluxHistogram->SetMarkerColor(kBlack);
  m_fluxHistogram->SetMarkerStyle(20);

  update(measurementTime);
}

FluxCalculation::~FluxCalculation()
{
}

TH1D* FluxCalculation::fluxHistogram() const
{
  return m_fluxHistogram;
}

void FluxCalculation::update(double measurementTime)
{
  const double acceptance = Constants::geometricAcceptance;
  const int nBins = m_particleHistogram->GetNbinsX();

  m_fluxHistogram->Reset();
  for (int i = 0 ; i < nBins; i++) {
    double content = m_particleHistogram->GetBinContent(i + 1);
    double error = m_particleHistogram->GetBinError(i + 1);
    m_fluxHistogram->SetBinContent(i + 1, content);
    m_fluxHistogram->SetBinError(i + 1, error);
  }

  m_fluxHistogram->Scale(1. / (acceptance * measurementTime), "width");
}

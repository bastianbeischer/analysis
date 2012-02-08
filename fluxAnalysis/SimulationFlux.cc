#include "SimulationFlux.hh"

#include "ParticleProperties.hh"

SimulationFlux::SimulationFlux(SimulationFluxKey key, TH2D* h2Spectrum)
  : m_key(key)
  , m_h2Spectrum(h2Spectrum)
{
}

SimulationFlux::~SimulationFlux()
{
}

TH1D* SimulationFlux::spectrum()
{
  double xMin = 0.;
  double xMax = 1.;

  if (m_key.isAlbedo()) {
    xMin = -1.;
    xMax = 0.;
  }

  TH1D* histogram = projectionY(m_h2Spectrum, xMin, xMax);
  scaleToBinWidth(histogram);
  histogram->SetTitle(qPrintable("MC " + ParticleProperties(m_key.particle()).name()));
  histogram->SetLineColor(ParticleProperties(m_key.particle()).color());
  histogram->SetMarkerColor(ParticleProperties(m_key.particle()).color());
  return histogram;
}

void SimulationFlux::scaleYToBinWidth(TH2D* histogram) {
  for(int i = 0; i < histogram->GetNbinsX(); i++) {
    for(int j = 0; j < histogram->GetNbinsY(); j++) {
      histogram->SetBinContent(i+1,j+1,((histogram->GetBinContent(i+1,j+1))/histogram->GetYaxis()->GetBinWidth(j+1)));
      histogram->SetBinError(i+1,j+1,((histogram->GetBinError(i+1,j+1))/histogram->GetYaxis()->GetBinWidth(j+1)));
    }
  }
}

TH1D* SimulationFlux::projectionY(TH2D* histogram, double xMin, double xMax) {
  QString projectionName = QString(histogram->GetName()) + "_projY_" + QString::number(xMin) + "_to_" + QString::number(xMax);
  int firstBin = histogram->GetXaxis()->FindBin(xMin);
  int lastBin = histogram->GetXaxis()->FindBin(xMax);
  TH1D* projHistogram = histogram->ProjectionY(qPrintable(projectionName), firstBin, lastBin);
  if (!projHistogram->GetSumw2())
    projHistogram->Sumw2();
  return projHistogram;
}

void SimulationFlux::scaleToBinWidth(TH1D* histogram) {
  if (!histogram->GetSumw2())
    histogram->Sumw2();
  histogram->Scale(1., "width");
}

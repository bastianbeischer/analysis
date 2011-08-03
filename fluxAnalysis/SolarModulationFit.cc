#include "SolarModulationFit.hh"

#include "Particle.hh"

#include <cmath>

double myFluxPhi(Double_t *x, Double_t *par)
{
  const int pdgId = 2212;
  const double z =  Particle(pdgId).charge();
  const double m = Particle(pdgId).mass();

  double R = x[0];
  double J0 = par[0];
  double gamma = par[1];
  double phi = par[2];

  double p = R * z;
  double E = sqrt(p*p + m*m);
  //  double T = E - m;
  double Phi = qAbs(z) * phi;
  double Jlis = J0 * pow(E+Phi, -gamma);

  double f = Jlis * (E*E - m*m) / (pow(E+Phi, 2) - m*m);
  return f;
}

SolarModulationFit::SolarModulationFit(TH1D* fluxHistogram) :
  m_fluxHistogram(fluxHistogram)
{
  QString title = fluxHistogram->GetTitle();

  const int nParams = 3;
  const double fitMin = 0.8;
  const double fitMax = 20;
  const QString fTitle = title + " phiFit";
  m_phiFit = new TF1(qPrintable(fTitle), myFluxPhi, fitMin, fitMax, nParams);
  m_phiFit->SetLineColor(kGreen);
  m_phiFit->SetParameters(0, 1.8*1e4);
  m_phiFit->SetParameters(1, 2.7);
  m_phiFit->SetParLimits(0, 1e3, 1e5);
  m_phiFit->SetParLimits(1, 1, 4);
  m_phiFit->SetParameters(2, 0.55);
  m_phiFit->SetParLimits(2, 0.1, 1.6);
}

SolarModulationFit::SolarModulationFit()
{
}

SolarModulationFit::~SolarModulationFit()
{
}

TF1* SolarModulationFit::fit()
{
  m_fluxHistogram->Fit(m_phiFit,"EQRN0");
  m_gamma = m_phiFit->GetParameter(1);
  m_gammaError = m_phiFit->GetParError(1);
  m_phi = m_phiFit->GetParameter(2) * 1e3;
  m_phiError = m_phiFit->GetParError(2) * 1e3;
  return m_phiFit;
}

QString SolarModulationFit::gammaLabel() const
{
  return QString("#gamma = %1 #pm %2").arg(m_gamma, 0, 'g', 3).arg(m_gammaError, 0, 'g', 0);
}

QString SolarModulationFit::phiLabel() const
{
  return QString("#phi / MV = %1 #pm %2").arg(m_phi, 0, 'g', 3).arg(m_phiError, 0, 'g', 2);
}


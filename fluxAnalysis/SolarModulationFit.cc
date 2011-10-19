#include "SolarModulationFit.hh"

#include "Particle.hh"

#include <TH1.h>
#include <TF1.h>

#include <QDebug>

#include <cmath>

double SolarModulationFit::powerLaw(double* x, double* par)
{
  const double z =  Particle(m_pdgId).charge();
  const double m = Particle(m_pdgId).mass();

  double R = x[0];
  double J0 = par[0];
  double gamma = par[1];
  double phi = par[2];
  double gammaC = par[3];
  double Rc = par[4];

  double p = R * z;
  double E = sqrt(p*p + m*m);
  double Phi = qAbs(z) * phi;
  double Jlis = J0 * pow(E+Phi, -gamma);
  double solarModulation = (E*E - m*m) / (pow(E+Phi, 2) - m*m);
  double geomagenticModulation = 1 / (1+pow((R/Rc), -gammaC));

  return Jlis * solarModulation * geomagenticModulation;
}

SolarModulationFit::SolarModulationFit(TH1D* fluxHistogram, int pdgId, double rangeMin, double rangeMax)
  : m_fluxHistogram(fluxHistogram)
  , m_pdgId(pdgId)
{
  QString title = fluxHistogram->GetTitle();
  const int nParams = 5;
  const QString fTitle = title + " phiFit";
  m_phiFit = new TF1(qPrintable(fTitle), this, &SolarModulationFit::powerLaw, rangeMin, rangeMax, nParams, "SolarModulationFit", "powerLaw");
  m_phiFit->SetLineColor(kRed);
  m_phiFit->SetLineWidth(3);
  //J0
  m_phiFit->SetParameter(0, 1e4);
  m_phiFit->SetParLimits(0, 1e2, 5e4);
  //gamma
  m_phiFit->SetParameter(1, 2.7);
  m_phiFit->SetParLimits(1, 2.4, 3);
  //phi
  m_phiFit->SetParameter(2, 0.55);
  m_phiFit->SetParLimits(2, 0.4, 0.7);
  //gammaC
  m_phiFit->SetParameter(3, 2);
  m_phiFit->SetParLimits(3, 1, 6);
  //Rc
  m_phiFit->SetParameter(4, 0.8);
  m_phiFit->SetParLimits(4, 0.5, 0.9);
}

SolarModulationFit::SolarModulationFit()
{
}

SolarModulationFit::~SolarModulationFit()
{
}

void SolarModulationFit::setGamma(double gamma)
{
  m_phiFit->FixParameter(1, gamma);
}

void SolarModulationFit::setGammaC(double gammaC)
{
  m_phiFit->FixParameter(3, gammaC);
}

void SolarModulationFit::setRc(double Rc)
{
  m_phiFit->FixParameter(4, Rc);
}

TF1* SolarModulationFit::fit()
{
  m_fluxHistogram->Fit(m_phiFit,"EQRN0");
  return m_phiFit;
}

QString SolarModulationFit::J0Label() const
{
  QString label = QString("#J_{0} / m^{−2} s^{−1} sr^{−1} GeV^{#gamma-1} = %1 ").arg(J0());
  if (J0Error() == 0)
    label.append("(fixed)");
  else
    label.append(QString("#pm %1").arg(J0Error()));
  return label;
}

QString SolarModulationFit::gammaLabel() const
{
  QString label = QString("#gamma = %1 ").arg(gamma(), 0, 'g', 3);
  if (gammaError() == 0)
    label.append("(fixed)");
  else
    label.append(QString("#pm %1").arg(gammaError(), 0, 'g', 0));
  return label;
}

QString SolarModulationFit::phiLabel() const
{
  QString label = QString("#phi / MV = %1 ").arg(phi(), 0, 'g', 3);
  if (phiError() == 0)
    label.append("(fixed)");
  else
    label.append(QString("#pm %1").arg(phiError(), 0, 'g', 2));
  return label;
}

QString SolarModulationFit::gammaClabel() const
{
  QString label = QString("#gamma_{C} = %1 ").arg(gammaC());
  if (gammaCerror() == 0)
    label.append("(fixed)");
  else
    label.append(QString("#pm %1").arg(gammaCerror()));
  return label;
}

QString SolarModulationFit::rClabel() const
{
  QString label = QString("R_{C} / GeV = %1 ").arg(rC());
  if (rCerror() == 0)
    label.append("(fixed)");
  else
    label.append(QString("#pm %1").arg(rCerror()));
  return label;
}

double SolarModulationFit::J0() const
{
  return m_phiFit->GetParameter(0);
}

double SolarModulationFit::J0Error() const
{
  return m_phiFit->GetParError(0);
}

double SolarModulationFit::gamma() const
{
  return m_phiFit->GetParameter(1);
}

double SolarModulationFit::gammaError() const
{
  return m_phiFit->GetParError(1);
}

double SolarModulationFit::phi() const
{
  return m_phiFit->GetParameter(2) * 1e3;
}

double SolarModulationFit::phiError() const
{
  return m_phiFit->GetParError(2) * 1e3;
}

double SolarModulationFit::gammaC() const
{
  return m_phiFit->GetParameter(3);
}

double SolarModulationFit::gammaCerror() const
{
  return m_phiFit->GetParError(3);
}

double SolarModulationFit::rC() const
{
  return m_phiFit->GetParameter(4);
}

double SolarModulationFit::rCerror() const
{
  return m_phiFit->GetParError(4);
}


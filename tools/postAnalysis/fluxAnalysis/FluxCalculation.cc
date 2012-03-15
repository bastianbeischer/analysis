#include "FluxCalculation.hh"

#include "Helpers.hh"
#include "Constants.hh"
#include "Enums.hh"
#include "EnumSelector.hh"
#include "FluxCalculation.hh"

#include <TH1.h>

#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>

FluxCalculation* FluxCalculation::s_instance = 0;

FluxCalculation* FluxCalculation::instance()
{
  if (!s_instance)
    s_instance = new FluxCalculation();
  return s_instance;
}

FluxCalculation::FluxCalculation()
  : m_measurementTime(-1.)
  , m_rigiditySpectra()
  , m_efficiencies()
{
}

FluxCalculation::~FluxCalculation()
{
  s_instance = 0;
}

void FluxCalculation::setMeasurementTime(double time)
{
  m_measurementTime = time;
}

void FluxCalculation::setRawSpectra(const QMap<Enums::Particle, const TH1D*>& spectra)
{
  m_rigiditySpectra = spectra;
}

void FluxCalculation::addEfficiency(Enums::Particle particle, TH1D* h)
{
  m_efficiencies.insert(particle, h);
}

double FluxCalculation::measurementTime() const
{
  return m_measurementTime;
}

Enums::Particles FluxCalculation::particles() const
{
  Enums::Particles particles = Enums::NoParticle;
  foreach (Enums::Particle particle, m_rigiditySpectra.keys())
    particles|= particle;
  return particles;
}

const QMap<Enums::Particle, const TH1D*>& FluxCalculation::rawSpectra() const
{
  return m_rigiditySpectra;
}

TH1D* FluxCalculation::newRawSpectrum(Enums::Particle particle) const
{
  QMap<Enums::Particle, const TH1D*>::ConstIterator it = m_rigiditySpectra.constFind(particle);
  if (it == m_rigiditySpectra.end())
    return 0;
  TH1D* h = static_cast<TH1D*>(it.value()->Clone());
  h->GetXaxis()->SetTitle("|R| / GV");
  h->GetYaxis()->SetTitle("flux / GV^{-1}m^{-2}s^{-1}sr^{-1}");
  return h;
}

TH1D* FluxCalculation::newFluxSpectrum(Enums::Particle particle) const
{
  if (m_measurementTime < 0)
    return 0;
  TH1D* h = newRawSpectrum(particle);
  h->Scale(1./m_measurementTime);
  h->Scale(1./Constants::geometricAcceptance);
  foreach (const TH1D* efficiency, m_efficiencies.values(particle)) {
    h->Divide(efficiency);
    qDebug() << "Efficiency correction for" << Enums::label(particle) << efficiency->GetName();
  }
  return h;
}

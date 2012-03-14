#include "FluxCalculation.hh"

#include "Helpers.hh"
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
  : m_rigiditySpectra()
  , m_measurementTime(-1.)
{
}

FluxCalculation::~FluxCalculation()
{
  s_instance = 0;
}

void FluxCalculation::setRawSpectra(QMap<Enums::Particle, const TH1D*> spectra)
{
  m_rigiditySpectra = spectra;
}

const QMap<Enums::Particle, const TH1D*>& FluxCalculation::rawSpectra() const
{
  return m_rigiditySpectra;
}

void FluxCalculation::setMeasurementTime(double time)
{
  m_measurementTime = time;
}

Enums::Particles FluxCalculation::particles() const
{
  Enums::Particles particles = Enums::NoParticle;
  foreach (Enums::Particle particle, m_rigiditySpectra.keys())
    particles|= particle;
  return particles;
}

double FluxCalculation::measurementTime() const
{
  return m_measurementTime;
}

TH1D* FluxCalculation::newRawSpectrum(Enums::Particle particle) const
{
  QMap<Enums::Particle, const TH1D*>::ConstIterator it = m_rigiditySpectra.constFind(particle);
  if (it == m_rigiditySpectra.end())
    return 0;
  return static_cast<TH1D*>(it.value()->Clone());
}

TH1D* FluxCalculation::newFluxSpectrum(Enums::Particle particle) const
{
  TH1D* h = newRawSpectrum(particle);
  //TODO
  return h;
}

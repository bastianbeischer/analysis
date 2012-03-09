#include "FluxCalculation.hh"

#include "Helpers.hh"
#include "Enums.hh"
#include "EnumSelector.hh"
#include "FluxCalculation.hh"

#include <TH1.h>

#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>

FluxCalculation::FluxCalculation(QMap<Enums::Particle, const TH1D*> rigiditySpectra)
  : m_rigiditySpectra(rigiditySpectra)
{
}

FluxCalculation::~FluxCalculation()
{
}

TH1D* FluxCalculation::newRawSpectrum(Enums::Particle particle)
{
  QMap<Enums::Particle, const TH1D*>::ConstIterator it = m_rigiditySpectra.constFind(particle);
  if (it == m_rigiditySpectra.end())
    return 0;
  return static_cast<TH1D*>(it.value()->Clone());
}

TH1D* FluxCalculation::newSummedRawSpectrum(Enums::Particles particles)
{
  TH1D* h = 0;
  for (Enums::ParticleIterator particleIt = Enums::particleBegin(); particleIt != Enums::particleEnd(); ++particleIt) {
    if (particleIt.key() == Enums::NoParticle)
      continue;
    if ((particleIt.key() & particles) != particleIt.key())
      continue;
    QMap<Enums::Particle, const TH1D*>::ConstIterator it = m_rigiditySpectra.constFind(particleIt.key());
    Q_ASSERT(it != m_rigiditySpectra.end());
    if (h) {
      h->Add(it.value());
    } else {
      h = static_cast<TH1D*>(it.value()->Clone());
      h->SetMarkerColor(kBlack);
      h->SetLineColor(kBlack);
    }
  }
  return h;
}

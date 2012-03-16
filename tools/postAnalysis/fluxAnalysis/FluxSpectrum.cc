#include "FluxSpectrum.hh"
#include "FluxCalculation.hh"
#include "SimulationFluxReader.hh"
#include "SimulationFluxSelector.hh"
#include "Helpers.hh"

#include "TPad.h"
#include "TH1.h"
#include "TLegend.h"

#include "QLayout"

FluxSpectrum::FluxSpectrum()
  : SpectrumPlot(&FluxCalculation::newFluxSpectrum)
  , m_simulationFluxSelector(new SimulationFluxSelector(QStringList() << "flux" << "summed flux"))
{
  setTitle("flux spectrum");
  setAxisTitle("|R| / GV", "flux / GV^{-1}m^{-2}s^{-1}sr^{-1}");
  static_cast<QHBoxLayout*>(secondaryWidget()->layout())->insertWidget(0, m_simulationFluxSelector);
  connect(m_simulationFluxSelector, SIGNAL(selectionChanged()), this, SLOT(update()));
}

FluxSpectrum::~FluxSpectrum()
{
}

void FluxSpectrum::addSimulationSpectra()
{
  TLegend* l = legend();

  QVector<SimulationFluxKey> keys = m_simulationFluxSelector->keys(0);
  foreach (SimulationFluxKey key, keys) {
    TH1D* h = SimulationFluxReader::instance()->spectrum(key);
    h->SetMarkerStyle(key.isAlbedo() ? 22 : 23);
    addHistogram(h);
    QString prefix = key.isAlbedo() ? "simulated albedo " : "simulated ";
    l->AddEntry(h, qPrintable(prefix + Helpers::greekifyLetters(Enums::label(key.particle()))));
  }

  QVector<SimulationFluxKey> sumKeys = m_simulationFluxSelector->keys(1);
  if (sumKeys.count()) {
    TH1D* h = SimulationFluxReader::instance()->spectrum(sumKeys);
    h->SetMarkerStyle(21);
    h->SetLineColor(kBlack);
    h->SetMarkerColor(kBlack);
    addHistogram(h);
    Enums::Particles particles = Enums::NoParticle;
    Enums::Particles albedoParticles = Enums::NoParticle;
    foreach (SimulationFluxKey key, sumKeys) {
      if (key.isAlbedo()) {
        albedoParticles|= key.particle();
      } else {
        particles|= key.particle();
      }
    }
    QString label;
    QString particleLabel = Helpers::greekifyLetters(Enums::label(particles));
    particleLabel.replace(" |", ",");
    QString albedoParticleLabel = Helpers::greekifyLetters(Enums::label(albedoParticles));
    albedoParticleLabel.replace(" |", ",");

    if (particles == Enums::NoParticle) {
      label = "sum of albedo " + albedoParticleLabel;
    } else if (albedoParticles == Enums::NoParticle) {
      label = "sum of " + particleLabel;
    } else {
      label = "#splitline{sum of " + particleLabel + "}{and albedo " + albedoParticleLabel + "}";
    }
    l->AddEntry(h, qPrintable(label));
  }
}

void FluxSpectrum::update()
{
  removeHistograms();
  addSpectra();
  addSimulationSpectra();
  resizeLegend();
  Q_ASSERT(m_canvas);
  H1DPlot::draw(m_canvas);
  gPad->Modified();
  gPad->Update();
}

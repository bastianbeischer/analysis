#include "SpectrumPlot.hh"

#include "Helpers.hh"
#include "Enums.hh"
#include "EnumSelector.hh"
#include "FluxCalculation.hh"

#include <TH1.h>
#include <TPad.h>

#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>

SpectrumPlot::SpectrumPlot(SpectrumFunctionPointer pointer)
  : QObject()
  , PostAnalysisPlot()
  , H1DPlot()
  , m_functionPointer(pointer)
  , m_canvas(0)
  , m_particleSelector(0)
  , m_sumSelector(0)
{
  Enums::Particles particles = FluxCalculation::instance()->particles();

  QHBoxLayout* layout = new QHBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addStretch();
  m_particleSelector = new ParticleSelector("particles", Enums::particleBegin(), Enums::particleEnd(), particles);
  layout->addWidget(m_particleSelector);
  m_sumSelector = new ParticleSelector("sum", Enums::particleBegin(), Enums::particleEnd(), particles);
  layout->addWidget(m_sumSelector);
  QWidget* widget = new QWidget();
  widget->setLayout(layout);
  setSecondaryWidget(widget);

  connect(m_particleSelector, SIGNAL(selectionChanged()), this, SLOT(update()));
  connect(m_sumSelector, SIGNAL(selectionChanged()), this, SLOT(update()));
}

SpectrumPlot::~SpectrumPlot()
{
}

TH1D* SpectrumPlot::newSummedSpectrum(Enums::Particles particles) const
{
  TH1D* sum = 0;
  for (Enums::ParticleIterator particleIt = Enums::particleBegin(); particleIt != Enums::particleEnd(); ++particleIt) {
    if (particleIt.key() == Enums::NoParticle)
      continue;
    if ((particleIt.key() & particles) != particleIt.key())
      continue;
    TH1D* h = (FluxCalculation::instance()->*m_functionPointer)(particleIt.key());
    Q_ASSERT(h);
    if (sum) {
      sum->Add(h);
      delete h;
    } else {
      h->SetMarkerColor(kBlack);
      h->SetLineColor(kBlack);
      sum = h;
    }
  }
  return sum;
}

void SpectrumPlot::draw(TCanvas* canvas)
{
  m_canvas = canvas;
  update();
}

void SpectrumPlot::update()
{
  removeHistograms();

  QVector<Enums::Particle> selectedParticles = m_particleSelector->selectedElementsVector();
  foreach (Enums::Particle particle, selectedParticles)
    addHistogram((FluxCalculation::instance()->*m_functionPointer)(particle));

  if (m_sumSelector->selectedElementsVector().count())
    addHistogram(newSummedSpectrum(m_sumSelector->selectedElements()));

  Q_ASSERT(m_canvas);
  H1DPlot::draw(m_canvas);

  gPad->Modified();
  gPad->Update();
}

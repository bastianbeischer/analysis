#include "RigidityFluxPlot.hh"

#include "Helpers.hh"
#include "Enums.hh"
#include "EnumSelector.hh"
#include "FluxCalculation.hh"

#include <TH1.h>

#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>

RigidityFluxPlot::RigidityFluxPlot(FluxCalculation* calculation)
  : QObject()
  , PostAnalysisPlot()
  , H1DPlot()
  , m_canvas(0)
  , m_calculation(calculation)
  , m_particleSelector(0)
  , m_sumSelector(0)
{
  setTitle("rigidity spectrum");
  QVector<Enums::Particle> particleVector = rigiditySpectra.keys().toVector();

  QHBoxLayout* layout = new QHBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addStretch();
  m_particleSelector = new ParticleSelector("particles", Enums::particleBegin(), Enums::particleEnd(), particleVector);
  layout->addWidget(m_particleSelector);
  m_sumSelector = new ParticleSelector("sum", Enums::particleBegin(), Enums::particleEnd(), particleVector);
  layout->addWidget(m_sumSelector);
  QWidget* widget = new QWidget();
  widget->setLayout(layout);
  setSecondaryWidget(widget);

  connect(m_particleSelector, SIGNAL(selectionChanged()), this, SLOT(update()));
  connect(m_sumSelector, SIGNAL(selectionChanged()), this, SLOT(update()));
}

RigidityFluxPlot::~RigidityFluxPlot()
{
}

void RigidityFluxPlot::draw(TCanvas* canvas)
{
  m_canvas = canvas;
  update();
}

void RigidityFluxPlot::update()
{
  removeHistograms();

  QVector<Enums::Particle> selectedParticles = m_particleSelector->selectedElementsVector();
  foreach (Enums::Particle particle, selectedParticles)
    addHistogram(m_calculation.newRawSpectrum(particle));

  if (m_sumSelector->selectedElementsVector().count())
    addHistogram(m_calculation.newSummedRawSpectrum(m_sumSelector->selectedElements()));

  Q_ASSERT(m_canvas);
  H1DPlot::draw(m_canvas);

  gPad->Modified();
  gPad->Update();
}

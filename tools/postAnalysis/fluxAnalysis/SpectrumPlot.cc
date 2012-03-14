#include "SpectrumPlot.hh"

#include "Helpers.hh"
#include "Enums.hh"
#include "EnumSelector.hh"
#include "FluxCalculation.hh"

#include <TH1.h>
#include <TPad.h>
#include <TLegend.h>

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
  m_particleSelector->selectAll();
  layout->addWidget(m_particleSelector);
  m_sumSelector = new ParticleSelector("sum", Enums::particleBegin(), Enums::particleEnd(), particles);
  layout->addWidget(m_sumSelector);
  QWidget* widget = new QWidget();
  widget->setLayout(layout);
  setSecondaryWidget(widget);


  TLegend* legend = new TLegend;
  legend->SetTextSize(0.02);
  legend->SetTextAlign(12);
  addLegend(legend);

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
  
  TLegend* l = legend();
  l->Clear();

  int entries = 0;

  QVector<Enums::Particle> selectedParticles = m_particleSelector->selectedElementsVector();
  foreach (Enums::Particle particle, selectedParticles) {
    TH1D* h = (FluxCalculation::instance()->*m_functionPointer)(particle);
    addHistogram(h);
    l->AddEntry(h, qPrintable(Helpers::greekifyLetters(Enums::label(particle))));
    ++entries;
  }

  if (m_sumSelector->selectedElementsVector().count()) {
    Enums::Particles particles = m_sumSelector->selectedElements();
    TH1D* h = newSummedSpectrum(particles);
    addHistogram(h);
    QString label = Helpers::greekifyLetters(Enums::label(particles));
    label.replace(" |", ",");
    l->AddEntry(h, qPrintable("sum of " + label));
    ++entries;
  }

  l->SetX1NDC(0.75);
  l->SetX2NDC(0.88);
  l->SetY1NDC(0.88 - 0.03 * entries);
  l->SetY2NDC(0.88);

  Q_ASSERT(m_canvas);
  H1DPlot::draw(m_canvas);

  gPad->Modified();
  gPad->Update();
}

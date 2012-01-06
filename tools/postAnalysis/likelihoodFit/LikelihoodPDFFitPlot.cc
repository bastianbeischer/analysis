#include "LikelihoodPDFFitPlot.hh"
#include "Likelihood.hh"
#include "LikelihoodPDF.hh"
#include "ParameterWidget.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TPad.h>

#include <QDebug>
#include <QHBoxLayout>
#include <QWidget>

LikelihoodPDFFitPlot::LikelihoodPDFFitPlot(const Likelihood* lh, const TH2D* h, int bin)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_currentFunction(0)
  , m_previewFunction(0)
  , m_parameterWidgets()
{
  double rangeMin = h->GetXaxis()->GetBinLowEdge(bin);
  double rangeMax = h->GetXaxis()->GetBinUpEdge(bin);
  double center = .5 * (rangeMin + rangeMax);

  Enums::Particles particles = lh->particles();

  QString particleLabel = (particles == Enums::NoParticle) ? "all particles" : Enums::label(particles);
  QString title = QString("%1 PDF %2..%3 GeV %4").arg(lh->title()).arg(rangeMin, 3, 'f', 1, '0')
    .arg(rangeMax, 3, 'f', 1, '0').arg(particleLabel);
  setTitle(title);

  for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it) {
    if (!(it.key() & particles))
      continue;
    addHistogram(h->ProjectionY(qPrintable(title), bin, bin));
    m_currentFunction = lh->pdf(KineticVariable(it.key(), Enums::AbsoluteRigidity, center));
    addFunction(m_currentFunction);
    m_previewFunction = lh->pdf(KineticVariable(it.key(), Enums::AbsoluteRigidity, center));
    addFunction(m_previewFunction);
  }

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);

  m_normalizationFactorWidget = new ParameterWidget();
  connect(m_normalizationFactorWidget, SIGNAL(valueChanged(double)), this, SLOT(update()));
  layout->addWidget(m_normalizationFactorWidget);

  for (int parameter = 0; parameter < lh->numberOfParameters(); ++parameter) {
    ParameterWidget* parameterWidget = new ParameterWidget();
    m_parameterWidgets.append(parameterWidget);
    connect(parameterWidget, SIGNAL(valueChanged(double)), this, SLOT(update()));
    layout->addWidget(parameterWidget);
  }

  layout->addStretch();
  QWidget* widget = new QWidget;
  widget->setLayout(layout);
  setSecondaryWidget(widget);
}

LikelihoodPDFFitPlot::~LikelihoodPDFFitPlot()
{
}

void LikelihoodPDFFitPlot::update()
{
  PDFParameters parameters;
  parameters.setNormalizationFactor(m_normalizationFactorWidget->value());
  for (int parameter = 0; parameter < m_parameterWidgets.count(); ++parameter)
    parameters.append(m_parameterWidgets[parameter]->value());
  m_previewFunction->setParameters(parameters);
  gPad->Modified();
  gPad->Update();
}

#include "LikelihoodPDFFitPlot.hh"
#include "Likelihood.hh"
#include "LikelihoodPDF.hh"
#include "ParameterWidget.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TPad.h>
#include <TLatex.h>

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>

LikelihoodPDFFitPlot::LikelihoodPDFFitPlot(Likelihood* lh, const TH2D* h, int bin)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_absoluteMomentum(0)
  , m_particle(Enums::NoParticle)
  , m_likelihood(lh)
  , m_currentFunction(0)
  , m_previewFunction(0)
  , m_parameterWidgets()
{
  double rangeMin = h->GetXaxis()->GetBinLowEdge(bin);
  double rangeMax = h->GetXaxis()->GetBinUpEdge(bin);
  m_absoluteMomentum = .5 * (rangeMin + rangeMax);

  Enums::Particles particles = lh->particles();

  QString particleLabel = (particles == Enums::NoParticle) ? "all particles" : Enums::label(particles);
  QString title = QString("%1 PDF %2..%3 GeV %4").arg(lh->title()).arg(rangeMin, 3, 'f', 1, '0')
    .arg(rangeMax, 3, 'f', 1, '0').arg(particleLabel);
  setTitle(title);

  TH1D* projection = h->ProjectionY(qPrintable(title), bin, bin);
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it) {
    if (!(it.key() & particles))
      continue;
    m_particle = it.key();
    addHistogram(projection);
    m_currentFunction = lh->pdf(KineticVariable(it.key(), Enums::AbsoluteRigidity, m_absoluteMomentum));
    addFunction(m_currentFunction);
    m_previewFunction = lh->pdf(KineticVariable(it.key(), Enums::AbsoluteRigidity, m_absoluteMomentum));
    addFunction(m_previewFunction);
    for (int i = 0; i < m_likelihood->numberOfParameters(); ++i)
      m_parameterWidgets.append(new ParameterWidget());
  }
  Q_ASSERT(m_particle != Enums::NoParticle);
  m_scaleWidget = new ParameterWidget();
  addLatex(RootPlot::newLatex(0.12, 0.88));
}

LikelihoodPDFFitPlot::~LikelihoodPDFFitPlot()
{
}

double LikelihoodPDFFitPlot::chi2() const
{
  double chi2 = 0.;
  int nBins = histogram()->GetXaxis()->GetNbins();
  for (int bin = 1; bin <= nBins; ++bin) {
    double y = histogram()->GetBinContent(bin);
    if (y > 0) {
      double x = histogram()->GetXaxis()->GetBinCenter(bin);
      double fy = m_previewFunction->Eval(x);
      double yError = histogram()->GetBinError(bin);
      chi2+= (y - fy) * (y - fy) / (yError * yError);
    }
  }
  return chi2;
}

int LikelihoodPDFFitPlot::ndf() const
{
  int ndf = -m_likelihood->numberOfParameters()-1;
  int nBins = histogram()->GetXaxis()->GetNbins();
  for (int bin = 1; bin <= nBins; ++bin)
    if (histogram()->GetBinContent(bin) > 0)
      ++ndf;
  return ndf;
}

bool LikelihoodPDFFitPlot::bad() const
{
  return !lowStatistics() && chi2() / ndf() > 3.0;
}

bool LikelihoodPDFFitPlot::good() const
{
  return !lowStatistics() && !bad();
}

bool LikelihoodPDFFitPlot::lowStatistics() const
{
  return histogram()->GetEntries() < 20.;
}

void LikelihoodPDFFitPlot::setup()
{
  m_previewFunction->SetLineStyle(kDashed);
  QHBoxLayout* hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 0, 0, 0);
  connect(m_scaleWidget, SIGNAL(valueChanged(double)), this, SLOT(update()));
  hLayout->addWidget(m_scaleWidget);
  for (int parameter = 0; parameter < m_likelihood->numberOfParameters(); ++parameter) {
    ParameterWidget* parameterWidget = m_parameterWidgets[parameter];
    connect(parameterWidget, SIGNAL(valueChanged(double)), this, SLOT(update()));
    hLayout->addWidget(parameterWidget);
  }
  hLayout->addStretch();
  QVBoxLayout* vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0, 0, 0, 0);
  QPushButton* button = 0;
  button = new QPushButton("fit");
  connect(button, SIGNAL(clicked()), this, SLOT(fit()));
  vLayout->addWidget(button);
  button = new QPushButton("reset");
  connect(button, SIGNAL(clicked()), this, SLOT(reset()));
  vLayout->addWidget(button);
  button = new QPushButton("save");
  connect(button, SIGNAL(clicked()), this, SLOT(save()));
  vLayout->addWidget(button);
  hLayout->addLayout(vLayout);
  QWidget* widget = new QWidget;
  widget->setLayout(hLayout);
  setSecondaryWidget(widget);

  updateLatex();
}

void LikelihoodPDFFitPlot::update()
{
  PDFParameters currentParameters = m_likelihood->interpolation(Hypothesis(m_particle, 1. / m_absoluteMomentum));
  m_currentFunction->setScaleFactor(m_scaleWidget->value());
  m_currentFunction->setParameters(currentParameters);
  PDFParameters previewParameters;
  m_previewFunction->setScaleFactor(m_scaleWidget->value());
  for (int parameter = 0; parameter < m_parameterWidgets.count(); ++parameter)
    previewParameters.append(m_parameterWidgets[parameter]->value());
  updateLatex();
  m_previewFunction->setParameters(previewParameters);
  gPad->Modified();
  gPad->Update();
}

void LikelihoodPDFFitPlot::updateLatex()
{
  int n = ndf();
  double c = chi2();
  QString title;
  if (n > 0) {
    title = QString("chi2 / ndf = %1 / %2 = %3").arg(c, 0, 'f', 2, ' ').arg(n).arg(c/n, 0, 'f', 2, ' ');
  } else {
    title = QString("chi2 = %1").arg(c, 0, 'f', 2, ' ');
  }
  latex()->SetTitle(qPrintable(title));
}

void LikelihoodPDFFitPlot::reset()
{
  const PDFParameters& parameters = m_currentFunction->parameters();
  for (int i = 0; i < parameters.count(); ++i)
    m_parameterWidgets[i]->setValue(parameters[i]);
}

void LikelihoodPDFFitPlot::save()
{
  m_likelihood->saveParameters(Hypothesis(m_particle, 1. / m_absoluteMomentum), m_previewFunction->parameters());
  emit configFileChanged();
}

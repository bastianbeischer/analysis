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
#include <QScrollArea>
#include <QLineEdit>
#include <QLabel>

LikelihoodPDFFitPlot::LikelihoodPDFFitPlot(Likelihood* lh, const TH2D* h, int bin, bool singleParticle)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_absoluteMomentum(0)
  , m_particle(Enums::particle(lh->particles()))
  , m_likelihood(lh)
  , m_currentFunction(0)
  , m_previewFunction(0)
  , m_parameterWidgets()
  , m_removeButton(new QPushButton("remove"))
  , m_currentParametersEdit(new QLineEdit())
{
  Q_ASSERT(m_particle != Enums::NoParticle);
  double rangeMin = h->GetXaxis()->GetBinLowEdge(bin);
  double rangeMax = h->GetXaxis()->GetBinUpEdge(bin);
  m_absoluteMomentum = .5 * (rangeMin + rangeMax);

  QString title = QString("%1 PDF %2..%3 GeV %4").arg(lh->title()).arg(rangeMin, 4, 'f', 2, '0')
    .arg(rangeMax, 4, 'f', 2, '0').arg(Enums::label(m_particle));
  if (!singleParticle)
    title+= " all";
  setTitle(title);

  KineticVariable variable(m_particle, Enums::AbsoluteRigidity, m_absoluteMomentum);

  TH1D* projection = h->ProjectionY(qPrintable(title), bin, bin);
  addHistogram(projection);
  m_currentFunction = lh->pdf(variable);
  addFunction(m_currentFunction);
  m_previewFunction = lh->pdf(variable);
  addFunction(m_previewFunction);
  for (int i = 0; i < m_likelihood->numberOfParameters(); ++i)
    m_parameterWidgets.append(new ParameterWidget());
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
  QScrollArea* scrollArea = new QScrollArea;
  QWidget* widget = new QWidget;
  widget->setContentsMargins(0, 0, 0, 0);
  widget->setLayout(hLayout);
  scrollArea->setWidget(widget);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  scrollArea->setFrameShape(QFrame::NoFrame);

  hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 0, 0, 0);
  hLayout->addWidget(scrollArea, 1);

  QGridLayout* gLayout = new QGridLayout();
  gLayout->setSpacing(3);
  gLayout->setContentsMargins(0, 0, 0, 0);

  int width = 60;

  QLabel* label = 0;

  label = new QLabel(Enums::label(m_particle));
  label->setAlignment(Qt::AlignCenter);
  label->setFixedWidth(width);
  gLayout->addWidget(label, 0, 0);

  label = new QLabel(QString("%1 GV").arg(m_absoluteMomentum, 4, 'f', 2, '0'));
  label->setAlignment(Qt::AlignCenter);
  label->setFixedWidth(width);
  gLayout->addWidget(label, 1, 0);

  QPushButton* button = 0;

  button = new QPushButton("fit");
  button->setFixedWidth(width);
  connect(button, SIGNAL(clicked()), this, SLOT(fit()));
  gLayout->addWidget(button, 0, 1);

  button = new QPushButton("reset");
  button->setFixedWidth(width);
  connect(button, SIGNAL(clicked()), this, SLOT(reset()));
  gLayout->addWidget(button, 1, 1);

  button = new QPushButton("save");
  button->setFixedWidth(width);
  connect(button, SIGNAL(clicked()), this, SLOT(save()));
  gLayout->addWidget(button, 0, 2);

  m_removeButton->setFixedWidth(width);
  connect(m_removeButton, SIGNAL(clicked()), this, SLOT(remove()));
  gLayout->addWidget(m_removeButton, 1, 2);

  int colSpan = 3;
  m_currentParametersEdit->setReadOnly(true);
  m_currentParametersEdit->setFixedWidth(colSpan * width + (colSpan-1) * gLayout->spacing());
  gLayout->addWidget(m_currentParametersEdit, 2, 0, 1, colSpan);

  hLayout->addLayout(gLayout, 0);
  widget = new QWidget;
  widget->setLayout(hLayout);
  widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  setSecondaryWidget(widget);

  update();
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

  int n = ndf();
  double c = chi2();
  QString title;
  if (n > 0) {
    title = QString("chi2 / ndf = %1 / %2 = %3").arg(c, 0, 'f', 2, ' ').arg(n).arg(c/n, 0, 'f', 2, ' ');
  } else {
    title = QString("chi2 = %1").arg(c, 0, 'f', 2, ' ');
  }
  latex()->SetTitle(qPrintable(title));
  m_removeButton->setEnabled(m_likelihood->parametersExist(Hypothesis(m_particle, 1./ m_absoluteMomentum)));
  m_previewFunction->setParameters(previewParameters);
  QString parameterString;
  foreach (double parameter, currentParameters)
    parameterString+= QString("%1, ").arg(parameter, 4, 'f', 2, '0');
  parameterString.chop(2);
  m_currentParametersEdit->setText(parameterString);
  m_currentParametersEdit->setCursorPosition(0);
  gPad->Modified();
  gPad->Update();
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

void LikelihoodPDFFitPlot::remove()
{
  m_likelihood->removeParameters(Hypothesis(m_particle, 1. / m_absoluteMomentum));
  emit configFileChanged();
}

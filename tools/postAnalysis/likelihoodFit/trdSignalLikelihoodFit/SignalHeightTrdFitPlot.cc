#include "SignalHeightTrdFitPlot.hh"
#include "PDFParameters.hh"
#include "Likelihood.hh"
#include "ParameterWidget.hh"
#include "LikelihoodPDF.hh"

#include <TH1.h>

#include <cmath>

SignalHeightTrdFitPlot::SignalHeightTrdFitPlot(Likelihood* lh, const TH2D* h, int bin)
  : LikelihoodPDFFitPlot(lh, h, bin)
{
  const PDFParameters& parameters = startParameters();
  Enums::Particle particle = Enums::particle(lh->particles());
  if (particle == Enums::Electron || particle == Enums::Positron) {
    m_parameterWidgets[0]->setRange(qMin(.0, parameters[0]), qMax(2., parameters[0]));
    m_parameterWidgets[1]->setRange(qMin(.0, parameters[1]), qMax(1., parameters[1]));
    m_parameterWidgets[2]->setRange(qMin(.0, parameters[2]), qMax(.3, parameters[2]));
    m_parameterWidgets[3]->setRange(qMin(.0, parameters[3]), qMax(1., parameters[3]));
    m_parameterWidgets[4]->setRange(qMin(.0, parameters[4]), qMax(3., parameters[4]));
    m_parameterWidgets[5]->setRange(qMin(.0, parameters[5]), qMax(1., parameters[5]));
    m_parameterWidgets[6]->setRange(qMin(.0, parameters[6]), qMax(3., parameters[6]));
    m_parameterWidgets[7]->setRange(qMin(-1., parameters[7]), qMax(.0, parameters[7]));
    m_parameterWidgets[8]->setRange(0, 0);
  } else {
    m_parameterWidgets[0]->setRange(qMin(.0, parameters[0]), qMax(25., parameters[0]));
    m_parameterWidgets[1]->setRange(qMin(.0, parameters[1]), qMax(2., parameters[1]));
    m_parameterWidgets[2]->setRange(qMin(.0, parameters[2]), qMax(.2, parameters[2]));
    m_parameterWidgets[3]->setRange(qMin(.0, parameters[3]), qMax(.2, parameters[3]));
    m_parameterWidgets[4]->setRange(qMin(-.5, parameters[4]), qMax(.0, parameters[4]));
    m_parameterWidgets[5]->setRange(qMin(.0, parameters[5]), qMax(.03, parameters[5]));
    m_parameterWidgets[6]->setRange(qMin(-.01, parameters[6]), qMax(.0, parameters[6]));
    m_parameterWidgets[7]->setRange(qMin(.0, parameters[7]), qMax(.2, parameters[7]));
    m_parameterWidgets[8]->setRange(qMin(.0, parameters[8]), qMax(.2, parameters[8]));
  }
  for (int i = 0; i < parameters.count(); ++i)
    m_parameterWidgets[i]->setValue(parameters[i]);
  m_previewFunction->setParameters(parameters);

  double max = qMax(1., histogram()->GetMaximum());
  max*= .5;
  m_scaleWidget->setRange(.0, 2. * max);
  m_scaleWidget->setValue(max);
  m_previewFunction->setScaleFactor(max);
  m_currentFunction->setScaleFactor(max);
  setup();
}

PDFParameters SignalHeightTrdFitPlot::startParameters() const
{
  Enums::Particle particle = Enums::particle(m_likelihood->particles());
  return m_likelihood->defaultParameters(particle);
}

void SignalHeightTrdFitPlot::fit()
{
  if (!lowStatistics()) {
    for (int parameter = 0; parameter < m_likelihood->numberOfParameters(); ++parameter)
      m_previewFunction->setLimits(parameter, m_parameterWidgets[parameter]->minimum(), m_parameterWidgets[parameter]->maximum());
    histogram()->Fit(m_previewFunction, "ERQN0");
    m_scaleWidget->disconnect();
    m_scaleWidget->setValue(m_previewFunction->scaleFactor());
    connect(m_scaleWidget, SIGNAL(valueChanged(double)), this, SLOT(update()));
    const PDFParameters& parameters = m_previewFunction->parameters();
    for (int parameter = 0; parameter < m_likelihood->numberOfParameters(); ++parameter) {
      m_parameterWidgets[parameter]->disconnect();
      m_parameterWidgets[parameter]->setValue(parameters[parameter]);
      connect(m_parameterWidgets[parameter], SIGNAL(valueChanged(double)), this, SLOT(update()));
    }
  }
  update();
}

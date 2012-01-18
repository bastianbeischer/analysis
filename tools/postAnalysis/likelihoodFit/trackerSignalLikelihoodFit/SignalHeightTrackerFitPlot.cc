#include "SignalHeightTrackerFitPlot.hh"
#include "PDFParameters.hh"
#include "Likelihood.hh"
#include "ParameterWidget.hh"
#include "LikelihoodPDF.hh"

#include <TH1.h>

#include <cmath>

SignalHeightTrackerFitPlot::SignalHeightTrackerFitPlot(Likelihood* lh, const TH2D* h, int bin)
  : LikelihoodPDFFitPlot(lh, h, bin)
{
  const PDFParameters& parameters = startParameters();

  m_parameterWidgets[0]->setRange(qMin(lh->measuredValueMin(), parameters[0]), qMax(lh->measuredValueMax(), parameters[0]));
  m_parameterWidgets[0]->setValue(parameters[0]);

  m_parameterWidgets[1]->setRange(qMin(0., parameters[1]), qMax(10., parameters[1]));
  m_parameterWidgets[1]->setValue(parameters[1]);

  m_previewFunction->setParameters(parameters);

  double max = qMax(1., histogram()->GetMaximum());
  max*= parameters[1] * sqrt(2 * M_PI);
  m_scaleWidget->setRange(0, 2 * max);
  m_scaleWidget->setValue(max);
  m_previewFunction->setScaleFactor(max);
  m_currentFunction->setScaleFactor(max);

  setup();
}

PDFParameters SignalHeightTrackerFitPlot::startParameters() const
{
  PDFParameters parameters(m_likelihood->numberOfParameters());
  parameters[0] = histogram()->GetMean();
  parameters[1] = histogram()->GetRMS();
  return parameters;
}

void SignalHeightTrackerFitPlot::fit()
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

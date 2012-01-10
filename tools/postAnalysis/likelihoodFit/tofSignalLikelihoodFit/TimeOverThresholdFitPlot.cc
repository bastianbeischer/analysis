#include "TimeOverThresholdFitPlot.hh"
#include "PDFParameters.hh"
#include "Likelihood.hh"
#include "ParameterWidget.hh"
#include "LikelihoodPDF.hh"

#include <TH1.h>

#include <cmath>

TimeOverThresholdFitPlot::TimeOverThresholdFitPlot(Likelihood* lh, const TH2D* h, int bin)
  : LikelihoodPDFFitPlot(lh, h, bin)
{
  const PDFParameters& parameters = lh->interpolation(Hypothesis(m_particle, 1./m_absoluteMomentum));

  m_parameterWidgets[0]->setRange(qMin(lh->measuredValueMin(), parameters[0]), qMax(lh->measuredValueMax(), parameters[0]));
  m_parameterWidgets[0]->setValue(parameters[0]);

  m_parameterWidgets[1]->setRange(qMin(0., parameters[1]), qMax(10., parameters[1]));
  m_parameterWidgets[1]->setValue(parameters[1]);

  double max = qMax(1., histogram()->GetMaximum());
  max*= parameters[1] * sqrt(2 * M_PI);
  m_scaleWidget->setRange(0, 2 * max);
  m_scaleWidget->setValue(max);
  m_previewFunction->setScaleFactor(max);
  m_currentFunction->setScaleFactor(max);

  setup();
}

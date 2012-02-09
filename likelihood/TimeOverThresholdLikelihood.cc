#include "TimeOverThresholdLikelihood.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"
#include "TimeOfFlight.hh"
#include "Helpers.hh"

#include <TMath.h>

#include <QDebug>

TimeOverThresholdLikelihood::TimeOverThresholdLikelihood(Enums::Particles particles)
  : Likelihood(particles)
{
  m_likelihoodVariableType = Enums::TimeOverThresholdLikelihood;
  m_measuredValueAxisTitle = "time-over-threshold / ns";
  m_measuredValueMin = 0.;
  m_measuredValueMax = 60.;
  m_numberOfParameters = 2;
  m_defaultParameters = PDFParameters() << 30.0 << 1.0;
  m_defaultParametersMap.insert(Enums::Helium, PDFParameters() << 40.0 << 5.0);
  m_title = Enums::label(m_likelihoodVariableType);
  loadParameters();
}

TimeOverThresholdLikelihood::~TimeOverThresholdLikelihood()
{
}

double TimeOverThresholdLikelihood::eval(const AnalyzedEvent* event, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  double signal = event->particle()->timeOfFlight()->timeOverThreshold();
  return eval(signal, hypothesis, goodInterpolation);
}

double TimeOverThresholdLikelihood::eval(double timeOverThreshold, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  const PDFParameters& parameters = interpolation(hypothesis, goodInterpolation);
  return eval(timeOverThreshold, hypothesis, parameters);
}

double TimeOverThresholdLikelihood::eval(double timeOverThreshold, const Hypothesis&, const PDFParameters& p) const
{
  double g1 = TMath::Gaus(timeOverThreshold, p[0], p[1], true);
  double g2 = TMath::Gaus(timeOverThreshold, p[0], 10., true);
  double fraction = .90;
  return fraction * g1 + (1 - fraction) * g2;
}

#include "SignalHeightTrackerLikelihood.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Helpers.hh"

#include <TMath.h>

#include <QDebug>

SignalHeightTrackerLikelihood::SignalHeightTrackerLikelihood(Enums::Particles particles)
  : Likelihood(particles)
{
  m_likelihoodVariableType = Enums::SignalHeightTrackerLikelihood;
  m_measuredValueAxisTitle = "tracker signal / ADC counts";
  m_measuredValueMin = 0.;
  m_measuredValueMax = 5000.;
  m_numberOfParameters = 2;
  m_defaultParameters = PDFParameters() << 1200.0 << 160.0;
  m_title = Enums::label(m_likelihoodVariableType);
  loadParameters();
}

SignalHeightTrackerLikelihood::~SignalHeightTrackerLikelihood()
{
}

double SignalHeightTrackerLikelihood::eval(const AnalyzedEvent* event, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  double signal = event->particle()->track()->signalHeight();
  return eval(signal, hypothesis, goodInterpolation);
}

double SignalHeightTrackerLikelihood::eval(double signalHeight, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  const PDFParameters& parameters = interpolation(hypothesis, goodInterpolation);
  return eval(signalHeight, hypothesis, parameters);
}

double SignalHeightTrackerLikelihood::eval(double signalHeight, const Hypothesis&, const PDFParameters& p) const
{
  double g1 = TMath::Student((signalHeight - p[0]) / p[1], 5.) / p[1];
  double g2 = TMath::Gaus(signalHeight, p[0], 100., true);
  double fraction = .90;
  return fraction * g1 + (1 - fraction) * g2;
}

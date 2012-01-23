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
  m_measuredValueType = Enums::UndefinedKineticVariable;
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
  return TMath::Gaus(signalHeight, parameters[0], parameters[1]);
}

double SignalHeightTrackerLikelihood::eval(double signalHeight, const Hypothesis&, const PDFParameters& parameters) const
{
  return TMath::Gaus(signalHeight, parameters[0], parameters[1], true);
}

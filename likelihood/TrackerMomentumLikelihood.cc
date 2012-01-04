#include "TrackerMomentumLikelihood.hh"
#include "Helpers.hh"
#include "Constants.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"

#include <TMath.h>

#include <QDebug>

TrackerMomentumLikelihood::TrackerMomentumLikelihood(Enums::Particles particles)
  : Likelihood(particles)
{
  m_likelihoodVariableType = Enums::TrackerMomentumLikelihood;
  m_measuredValueType = Enums::Curvature;
  m_measuredValueMin = -10.;
  m_measuredValueMax = +10.;
  m_numberOfParameters = 0;
  m_defaultParameters = PDFParameters(m_numberOfParameters);
  m_title = Enums::label(m_likelihoodVariableType);
}

TrackerMomentumLikelihood::~TrackerMomentumLikelihood()
{
}

double TrackerMomentumLikelihood::eval(const AnalyzedEvent* event, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  double curvature = 1. / event->particle()->track()->rigidity(); // measured by tracker only
  return eval(curvature, hypothesis, goodInterpolation);
}

double TrackerMomentumLikelihood::eval(double curvature, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  if (goodInterpolation)
    *goodInterpolation = true;
  if (curvature < m_measuredValueMin || curvature > m_measuredValueMax)
    return 0;
  if (hypothesis.charge() * hypothesis.curvature() < 0)
    return 0;
  double relativeSigma = Helpers::addQuad(0.08 * hypothesis.rigidity(), 0.25 * hypothesis.beta());
  return TMath::Gaus(curvature, hypothesis.curvature(), relativeSigma, true);
}

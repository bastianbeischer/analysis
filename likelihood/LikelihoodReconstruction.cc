#include "LikelihoodReconstruction.hh"
#include "Likelihood.hh"
#include "KineticVariable.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"
#include "Setup.hh"
#include "Helpers.hh"

#include <Math/IFunction.h>
#include <Math/BrentMinimizer1D.h>
#include <Math/Functor.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>

#include <cfloat>
#include <cmath>

class LogLikelihoodFunction : public ROOT::Math::IGenFunction {
public:
  typedef double (LikelihoodReconstruction::*Function)(const AnalyzedEvent*, const Hypothesis&, bool*) const;

  LogLikelihoodFunction(const LikelihoodReconstruction* analysis, Function function)
    : ROOT::Math::IGenFunction()
    , m_analysis(analysis)
    , m_function(function)
  {
  }

  virtual LogLikelihoodFunction* Clone() const
  {
    LogLikelihoodFunction* f = new LogLikelihoodFunction(m_analysis, m_function);
    f->setParameters(m_event, m_particleType, m_goodInterpolation);
    return f;
  }

  virtual double DoEval(double x) const
  {
    Q_ASSERT(m_event && m_analysis);
    return (m_analysis->*m_function)(m_event, Hypothesis(m_particleType, x), m_goodInterpolation);
  }

  void setParameters(const AnalyzedEvent* event, Enums::Particle particleType, bool* goodInterpolation)
  {
    m_event = event;
    m_particleType = particleType;
    m_goodInterpolation = goodInterpolation;
  }

private:
  const LikelihoodReconstruction* m_analysis;
  Function m_function;
  const AnalyzedEvent* m_event;
  Enums::Particle m_particleType;
  bool* m_goodInterpolation;
};

LikelihoodReconstruction::LikelihoodReconstruction(Enums::LikelihoodVariables likelihoods, Enums::Particles particles, bool additionalInformation)
  : Reconstruction(likelihoods, particles)
  , m_lhMinimizer(new ROOT::Math::BrentMinimizer1D)
  , m_lhFunction(new LogLikelihoodFunction(this, &Reconstruction::eval))
{
  m_externalInformation = additionalInformation;
  m_method = additionalInformation ? Enums::LikelihoodExternalInformation : Enums::Likelihood;
  m_lhMinimizer->SetFunction(*m_lhFunction, s_minimumCurvature, s_maximumCurvature);
  m_minima = QVector<QPointF>(m_particles.count());
}

LikelihoodReconstruction::~LikelihoodReconstruction()
{
  delete m_lhMinimizer;
  delete m_lhFunction;
}

void LikelihoodReconstruction::identify(AnalyzedEvent* event)
{
  m_event = event;
  bool goodInterpolation = true;
  m_indexOfGlobalMinimum = 0;

  QVector<QPointF>::Iterator pointIt = m_minima.begin();
  QVector<QPointF>::Iterator pointEnd = m_minima.end();
  for (; pointIt != pointEnd; ++pointIt)
    pointIt->setY(DBL_MAX);

  pointIt = m_minima.begin();
  QVector<Enums::Particle>::ConstIterator particleIt = m_particles.begin();
  QVector<Enums::Particle>::ConstIterator particleEnd = m_particles.end();
  for (int it = 0; particleIt != particleEnd; ++it, ++particleIt, ++pointIt) {
    Q_ASSERT(*particleIt != Enums::NoParticle);
    Q_ASSERT(pointIt != m_minima.end());
    m_lhFunction->setParameters(event, *particleIt, &goodInterpolation);
    if (m_lhMinimizer->Minimize(100)) {
      pointIt->setX(m_lhMinimizer->XMinimum());
      pointIt->setY(m_lhMinimizer->FValMinimum());
      if (it > 0 && m_lhMinimizer->FValMinimum() < m_minima[m_indexOfGlobalMinimum].y())
        m_indexOfGlobalMinimum = it;
      Hypothesis* h = new Hypothesis(*particleIt, m_lhMinimizer->XMinimum());
      h->setLogLikelihood(m_lhMinimizer->FValMinimum());
      event->particle()->addHypothesis(m_method, h);
    } else {
      pointIt->setX(0);
      pointIt->setY(0);
    }
  }
}

TLegend* LikelihoodReconstruction::legend() const
{
  setupDefaultLegend();
  return m_legend;
}

TMultiGraph* LikelihoodReconstruction::graph() const
{
  if (m_graph)
    delete m_graph;
  m_graph = new TMultiGraph;
  m_graph->SetTitle(";K / 1/GV;-2 ln L");
  double kStep = 0.01;
  int nSteps = (s_maximumCurvature - s_minimumCurvature) / kStep;

  foreach (Enums::Particle type, m_particles) {
    TGraph* g = new TGraph(nSteps);
    g->SetName(qPrintable(Enums::label(type)));
    g->SetLineColor(Particle(type).color());
    g->SetMarkerSize(0.1);
    g->SetLineWidth(2);
    for (int i = 0; i < nSteps; ++i) {
      Hypothesis hypothesis(type, s_minimumCurvature + i * kStep);
      g->SetPoint(i, hypothesis.curvature(), eval(m_event, hypothesis));
    }
    m_graph->Add(g, "L");
  }

  TGraph* minimaGraph = new TGraph;
  minimaGraph->SetMarkerStyle(20);
  for (int i = 0; i < m_minima.count(); ++i)
    if (i != m_indexOfGlobalMinimum)
      minimaGraph->SetPoint(minimaGraph->GetN(), m_minima[i].x(), m_minima[i].y());
  if (minimaGraph->GetN())
    m_graph->Add(minimaGraph, "P");

  TGraph* minimumGraph = new TGraph(1);
  minimumGraph->SetMarkerStyle(20);
  minimumGraph->SetMarkerColor(kRed);
  minimumGraph->SetPoint(0, m_minima[m_indexOfGlobalMinimum].x(), m_minima[m_indexOfGlobalMinimum].y());
  m_graph->Add(minimumGraph, "P");
  m_graph->SetTitle(qPrintable(QString(Enums::label(m_variables)).remove(" likelihood")));
  return m_graph;
}

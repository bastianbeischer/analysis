#include "Chi2Reconstruction.hh"
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

class Chi2Function : public ROOT::Math::IGenFunction {
public:
  Chi2Function()
    : m_event(0)
    , m_properties(Enums::NoParticle)
  {
  }

  virtual Chi2Function* Clone() const
  {
    Chi2Function* f = new Chi2Function();
    f->setParameters(m_event, m_properties.type());
    return f;
  }

  virtual double DoEval(double x) const
  {
    Q_ASSERT(m_event);
    double tracker = 0;
    double rigidity = m_event->particle()->track()->rigidity();
    if (m_properties.charge() * rigidity < 0)
      return 1e9;
    if (!qIsNull(rigidity)) {
      double curvature = 1. / rigidity;
      double sigmaCurvature = curvature * Helpers::trackerResolution(curvature, m_properties.type());
      tracker = (x - curvature) * (x - curvature) / (sigmaCurvature * sigmaCurvature);
    }
    double tof = 0;
    double beta = m_event->particle()->beta();
    if (!qIsNull(beta)) {
      double inverseBeta = 1. / beta;
      double sigmaInverseBeta = Helpers::tofResolution();
      double y = Helpers::addQuad(1., m_properties.mass() * x / m_properties.charge());
      tof = (y - inverseBeta) * (y - inverseBeta) / (sigmaInverseBeta * sigmaInverseBeta);
    }
    return tracker + tof;
  }

  void setParameters(const AnalyzedEvent* event, Enums::Particle particle)
  {
    m_event = event;
    m_properties = ParticleProperties(particle);
  }
private:
  const AnalyzedEvent* m_event;
  ParticleProperties m_properties;
};

Chi2Reconstruction::Chi2Reconstruction(Enums::LikelihoodVariables likelihoods, Enums::Particles particles, bool additionalInformation)
  : Reconstruction(likelihoods, particles)
  , m_chi2Minimizer(new ROOT::Math::BrentMinimizer1D)
  , m_chi2Function(new Chi2Function)
{
  m_externalInformation = additionalInformation;
  m_method = additionalInformation ? Enums::Chi2 : Enums::Chi2ExternalInformation;
  m_minima = QVector<QPointF>(m_particles.count());
  m_chi2Minimizer->SetFunction(*m_chi2Function, -10, 10);
}

Chi2Reconstruction::~Chi2Reconstruction()
{
  delete m_chi2Minimizer;
  delete m_chi2Function;
}

void Chi2Reconstruction::identify(AnalyzedEvent* event)
{
  m_event = event;
  bool goodInterpolation = true;
  m_indexOfGlobalMinimum = -1;

  QVector<QPointF>::Iterator pointIt = m_minima.begin();
  QVector<QPointF>::Iterator pointEnd = m_minima.end();
  for (; pointIt != pointEnd; ++pointIt)
    pointIt->setY(DBL_MAX);

  double min = DBL_MAX;

  pointIt = m_minima.begin();
  QVector<Enums::Particle>::ConstIterator particleIt = m_particles.begin();
  QVector<Enums::Particle>::ConstIterator particleEnd = m_particles.end();
  for (int it = 0; particleIt != particleEnd; ++it, ++particleIt, ++pointIt) {
    Q_ASSERT(*particleIt != Enums::NoParticle);
    Q_ASSERT(pointIt != m_minima.end());
    // minimize: (K - 1/p)^2 / (sigma_1/p)^2 + (sqrt((mK)^2 + 1) - 1/beta)^2 / (sigma_1/beta)^2
    m_chi2Function->setParameters(event, *particleIt);
    if (m_chi2Minimizer->Minimize(100)) {
      pointIt->setX(m_chi2Minimizer->XMinimum());
      pointIt->setY(m_chi2Minimizer->FValMinimum());
      Hypothesis* h = new Hypothesis(*particleIt, m_chi2Minimizer->XMinimum());
      double value = eval(event, *h, &goodInterpolation);
      if (value < min) {
        m_indexOfGlobalMinimum = it;
        min = value;
      }
      h->setLikelihood(m_likelihoods.count(), value);
      event->particle()->addHypothesis(m_method, h);
    } else {
      pointIt->setX(0);
      pointIt->setY(0);
    }
  }
}

TLegend* Chi2Reconstruction::legend() const
{
  setupDefaultLegend();
  return m_legend;
}

TMultiGraph* Chi2Reconstruction::graph() const
{
  if (m_graph)
    delete m_graph;
  m_graph = new TMultiGraph;
  m_graph->SetTitle(";K / 1/GV; #chi^{2}");
  double kMin = -10.0;
  double kMax = +10.0;
  double kStep = 0.01;
  int nSteps = (kMax - kMin) / kStep;

  foreach (Enums::Particle type, m_particles) {
    m_chi2Function->setParameters(m_event, type);
    TGraph* g = new TGraph(nSteps);
    g->SetName(qPrintable(Enums::label(type)));
    g->SetLineColor(Particle(type).color());
    g->SetMarkerSize(0.1);
    g->SetLineWidth(2);
    for (int i = 0; i < nSteps; ++i) {
      double curvature = kMin + i * kStep;
      g->SetPoint(i, curvature, m_chi2Function->DoEval(curvature));
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
  if (0 <= m_indexOfGlobalMinimum)
    minimumGraph->SetPoint(0, m_minima[m_indexOfGlobalMinimum].x(), m_minima[m_indexOfGlobalMinimum].y());
  m_graph->Add(minimumGraph, "P");
  m_graph->SetTitle(qPrintable(QString(Enums::label(m_variables)).remove(" likelihood")));
  return m_graph;
}

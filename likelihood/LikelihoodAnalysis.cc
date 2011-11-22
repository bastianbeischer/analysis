#include "LikelihoodAnalysis.hh"
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
      return 1e10;
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
      double y = Helpers::addQuad(1., m_properties.mass() * x);
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

class LogLikelihoodFunction : public ROOT::Math::IGenFunction {
public:
  typedef double (LikelihoodAnalysis::*Function)(const AnalyzedEvent*, const Hypothesis&, bool*) const;

  LogLikelihoodFunction(const LikelihoodAnalysis* analysis, Function function)
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
  const LikelihoodAnalysis* m_analysis;
  Function m_function;
  const AnalyzedEvent* m_event;
  Enums::Particle m_particleType;
  bool* m_goodInterpolation;
};

LikelihoodAnalysis::LikelihoodAnalysis(Enums::ReconstructionMethod method, Enums::LikelihoodVariables likelihoods, Enums::Particles particles)
  : m_method(method)
  , m_likelihoods()
  , m_particles()
  , m_chi2Minimizer(new ROOT::Math::BrentMinimizer1D)
  , m_lhMinimizer(new ROOT::Math::BrentMinimizer1D)
  , m_chi2Function(new Chi2Function())
  , m_lhFunction(new LogLikelihoodFunction(this, &LikelihoodAnalysis::eval))
  , m_chi2Minima()
  , m_lhMinima()
  , m_indexOfGlobalChi2Minimum(-1)
  , m_indexOfGlobalLhMinimum(-1)
  , m_event(0)
  , m_chi2MultiGraph(0)
  , m_lhMultiGraph(0)
{
  m_chi2Minimizer->SetFunction(*m_chi2Function, -10, 10);
  m_lhMinimizer->SetFunction(*m_lhFunction, -10, 10);

  for (Enums::LikelihoodVariableIterator it = Enums::likelihoodVariableBegin(); it != Enums::likelihoodVariableEnd(); ++it)
    if ((it.key() & likelihoods) == it.key())
      m_likelihoods.append(Likelihood::newLikelihood(it.key(), particles));

  for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it)
    if (it.key() != Enums::NoParticle && (it.key() & particles) == it.key())
      m_particles.append(it.key());
  m_chi2Minima = QVector<QPointF>(m_particles.count());
  m_lhMinima = QVector<QPointF>(m_particles.count());
}

LikelihoodAnalysis::~LikelihoodAnalysis()
{
  if (m_chi2MultiGraph)
    delete m_chi2MultiGraph;
  if (m_lhMultiGraph)
    delete m_lhMultiGraph;
  qDeleteAll(m_likelihoods);
}

int LikelihoodAnalysis::indexOfGlobalChi2Minimum() const
{
  return m_indexOfGlobalChi2Minimum;
}

int LikelihoodAnalysis::indexOfGlobalLikelihoodMinimum() const
{
  return m_indexOfGlobalLhMinimum;
}

const QVector<QPointF>& LikelihoodAnalysis::chi2Minima() const
{
  return m_chi2Minima;
}

const QVector<QPointF>& LikelihoodAnalysis::likelihoodMinima() const
{
  return m_lhMinima;
}

const QVector<Enums::Particle>& LikelihoodAnalysis::particles() const
{
  return m_particles;
}

void LikelihoodAnalysis::identify(AnalyzedEvent* event)
{
  m_event = event;
  bool goodInterpolation = true;
  m_indexOfGlobalChi2Minimum = 0;
  m_indexOfGlobalLhMinimum = 0;

  QVector<Enums::Particle>::ConstIterator particleIt = m_particles.begin();
  QVector<Enums::Particle>::ConstIterator particleEnd = m_particles.end();
  QVector<QPointF>::Iterator chi2PointIt = m_chi2Minima.begin();
  QVector<QPointF>::Iterator lhPointIt = m_lhMinima.begin();
  for (int it = 0; particleIt != particleEnd; ++it, ++particleIt, ++chi2PointIt, ++lhPointIt) {
    Q_ASSERT(*particleIt != Enums::NoParticle);
    if (m_method == Enums::Spectrometer || m_method == Enums::SpectrometerExternalInformation) {
      double curvature = 1. / event->particle()->track()->rigidity();
      Hypothesis* h = new Hypothesis(*particleIt, curvature);
      h->setProbability(1); //TODO
      event->particle()->addHypothesis(m_method, h);
    } else if (m_method == Enums::TOF || m_method == Enums::TOFExternalInformation) {
      double beta = event->particle()->beta();
      double beta2 = beta * beta;
      double mass = ParticleProperties(*particleIt).mass();
      double mass2 = mass * mass;
      double curvature = (1. - beta2) / (mass2 * beta2);
      Hypothesis* h = new Hypothesis(*particleIt, curvature);
      h->setProbability(1); //TODO
      event->particle()->addHypothesis(m_method, h);
    } else if (m_method == Enums::WeightedMean || m_method == Enums::WeightedMeanExternalInformation) {
      double enumeratur = 0;
      double denumerator = 0;
      double rigidity = event->particle()->track()->rigidity();
      if (!qIsNull(rigidity)) {
        double curvature = 1. / event->particle()->track()->rigidity();
        double relativeSigmaCurvature = Helpers::trackerResolution(curvature, *particleIt);
        double sigmaCurvature = curvature * relativeSigmaCurvature;
        double sigmaCurvature2 = sigmaCurvature * sigmaCurvature;
        enumeratur+= curvature/sigmaCurvature2;
        denumerator+= 1./sigmaCurvature2;
      }
      double inverseBeta = 1. / event->particle()->beta();
      if (inverseBeta > 1) {
        double tofResolution = Helpers::tofResolution();
        double mass = ParticleProperties(*particleIt).mass();
        double curvatureTof = sqrt(inverseBeta*inverseBeta - 1) / mass;
        double sigmaCurvatureTof = tofResolution * inverseBeta / sqrt(inverseBeta*inverseBeta - 1);
        double sigmaCurvatureTof2 = sigmaCurvatureTof * sigmaCurvatureTof;
        enumeratur+= curvatureTof/sigmaCurvatureTof2;
        denumerator+= 1./sigmaCurvatureTof2;
      }
      Hypothesis* h = new Hypothesis(*particleIt, qIsNull(denumerator) ? 0 : enumeratur / denumerator);
      h->setProbability(1); //TODO
      event->particle()->addHypothesis(m_method, h);
    } else if (m_method == Enums::Chi2 || m_method == Enums::Chi2ExternalInformation) {
      // minimize: (K - 1/p)^2 / (sigma_1/p)^2 + (sqrt((mK)^2 + 1) - 1/beta)^2 / (sigma_1/beta)^2 
      m_chi2Function->setParameters(event, *particleIt);
      if (m_chi2Minimizer->Minimize(100)) {
        chi2PointIt->setX(m_chi2Minimizer->XMinimum());
        chi2PointIt->setY(m_chi2Minimizer->FValMinimum());
        if (it > 0 && m_chi2Minimizer->FValMinimum() < m_chi2Minima[m_indexOfGlobalChi2Minimum].y())
          m_indexOfGlobalChi2Minimum = it;
        Hypothesis* h = new Hypothesis(*particleIt, m_chi2Minimizer->XMinimum());
        h->setProbability(1); //TODO
        event->particle()->addHypothesis(m_method, h);
      } else {
        chi2PointIt->setX(0);
        chi2PointIt->setY(0);
      }
    } else if (m_method == Enums::Likelihood || m_method == Enums::LikelihoodExternalInformation) {
      m_lhFunction->setParameters(event, *particleIt, &goodInterpolation);
      if (m_lhMinimizer->Minimize(100)) {
        lhPointIt->setX(m_lhMinimizer->XMinimum());
        lhPointIt->setY(m_lhMinimizer->FValMinimum());
        if (it > 0 && m_lhMinimizer->FValMinimum() < m_lhMinima[m_indexOfGlobalLhMinimum].y())
          m_indexOfGlobalLhMinimum = it;
        Hypothesis* h = new Hypothesis(*particleIt, m_lhMinimizer->XMinimum());
        h->setProbability(1); //TODO
        event->particle()->addHypothesis(m_method, h);
      } else {
        lhPointIt->setX(0);
        lhPointIt->setY(0);
      }
    }
  }
}

double LikelihoodAnalysis::eval(const AnalyzedEvent* event, const Hypothesis& hypothesis, bool* goodInterpolation) const
{
  double lnL = 0;
  QVector<Likelihood*>::ConstIterator end = m_likelihoods.end();
  for (QVector<Likelihood*>::ConstIterator it = m_likelihoods.begin(); it != end; ++it) {
    double value = (*it)->eval(event, hypothesis, goodInterpolation);
    if (qIsNull(value))
      return 1e10; //infinity
    lnL+= -2*log(value);
  }
  return lnL;
}

TMultiGraph* LikelihoodAnalysis::chi2Graph() const
{
  if (m_method != Enums::Chi2 && m_method != Enums::Chi2ExternalInformation) {
    qDebug() << "LikelihoodAnalysis::chi2Graph() called, but method != Chi2";
    return 0;
  }
  if (m_chi2MultiGraph)
    delete m_chi2MultiGraph;
  m_chi2MultiGraph = new TMultiGraph;
  m_chi2MultiGraph->SetTitle(";K / 1/GV; #chi^2");
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
    m_chi2MultiGraph->Add(g, "L");
  }

  TGraph* minimaGraph = new TGraph;
  minimaGraph->SetMarkerStyle(20);
  for (int i = 0; i < m_chi2Minima.count(); ++i)
    if (i != m_indexOfGlobalChi2Minimum)
      minimaGraph->SetPoint(minimaGraph->GetN(), m_chi2Minima[i].x(), m_chi2Minima[i].y());
  if (minimaGraph->GetN())
    m_chi2MultiGraph->Add(minimaGraph, "P");

  TGraph* minimumGraph = new TGraph(1);
  minimumGraph->SetMarkerStyle(20);
  minimumGraph->SetMarkerColor(kRed);
  minimumGraph->SetPoint(0, m_chi2Minima[m_indexOfGlobalChi2Minimum].x(), m_chi2Minima[m_indexOfGlobalChi2Minimum].y());
  m_chi2MultiGraph->Add(minimumGraph, "P");

  return m_chi2MultiGraph;
}

TMultiGraph* LikelihoodAnalysis::likelihoodGraph() const
{
  if (m_method != Enums::Likelihood && m_method != Enums::LikelihoodExternalInformation) {
    qDebug() << "LikelihoodAnalysis::likelihoodGraph() called, but method != Likelihood";
    return 0;
  }
  if (m_lhMultiGraph)
    delete m_lhMultiGraph;
  m_lhMultiGraph = new TMultiGraph;
  m_lhMultiGraph->SetTitle(";K / 1/GV;-2 ln L");
  double kMin = -10.0;
  double kMax = +10.0;
  double kStep = 0.01;
  int nSteps = (kMax - kMin) / kStep;

  foreach (Enums::Particle type, m_particles) {
    TGraph* g = new TGraph(nSteps);
    g->SetName(qPrintable(Enums::label(type)));
    g->SetLineColor(Particle(type).color());
    g->SetMarkerSize(0.1);
    g->SetLineWidth(2);
    for (int i = 0; i < nSteps; ++i) {
      Hypothesis hypothesis(type, kMin + i * kStep);
      g->SetPoint(i, hypothesis.curvature(), eval(m_event, hypothesis));
    }
    m_lhMultiGraph->Add(g, "L");
  }

  TGraph* minimaGraph = new TGraph;
  minimaGraph->SetMarkerStyle(20);
  for (int i = 0; i < m_lhMinima.count(); ++i)
    if (i != m_indexOfGlobalLhMinimum)
      minimaGraph->SetPoint(minimaGraph->GetN(), m_lhMinima[i].x(), m_lhMinima[i].y());
  if (minimaGraph->GetN())
    m_lhMultiGraph->Add(minimaGraph, "P");

  TGraph* minimumGraph = new TGraph(1);
  minimumGraph->SetMarkerStyle(20);
  minimumGraph->SetMarkerColor(kRed);
  minimumGraph->SetPoint(0, m_lhMinima[m_indexOfGlobalLhMinimum].x(), m_lhMinima[m_indexOfGlobalLhMinimum].y());
  m_lhMultiGraph->Add(minimumGraph, "P");

  return m_lhMultiGraph;
}

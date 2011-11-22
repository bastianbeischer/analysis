#ifndef LikelihoodAnalysis_hh
#define LikelihoodAnalysis_hh

#include "Enums.hh"
#include "Hypothesis.hh"

#include <QVector>
#include <QPointF>

class AnalyzedEvent;
class Likelihood;
class TMultiGraph;
class LogLikelihoodFunction;
class Chi2Function;
class LikelihoodPDF;
namespace ROOT {namespace Math {class BrentMinimizer1D;}}

class LikelihoodAnalysis {
public:
  LikelihoodAnalysis(Enums::ReconstructionMethod, Enums::LikelihoodVariables, Enums::Particles);
  ~LikelihoodAnalysis();
  int indexOfGlobalChi2Minimum() const;
  int indexOfGlobalLikelihoodMinimum() const;
  const QVector<QPointF>& chi2Minima() const;
  const QVector<QPointF>& likelihoodMinima() const;
  const QVector<Enums::Particle>& particles() const;

  void identify(AnalyzedEvent*);
  TMultiGraph* likelihoodGraph() const;
  TMultiGraph* chi2Graph() const;
private:
  double eval(const AnalyzedEvent*, const Hypothesis&, bool* goodInterpolation = 0) const;
  double rootFunctorPointer(const double* momentum);

  Enums::ReconstructionMethod m_method;
  QVector<Likelihood*> m_likelihoods;
  QVector<Enums::Particle> m_particles;
  ROOT::Math::BrentMinimizer1D* m_chi2Minimizer;
  ROOT::Math::BrentMinimizer1D* m_lhMinimizer;
  Chi2Function* m_chi2Function;
  LogLikelihoodFunction* m_lhFunction;
  QVector<QPointF> m_chi2Minima;
  QVector<QPointF> m_lhMinima;
  int m_indexOfGlobalChi2Minimum;
  int m_indexOfGlobalLhMinimum;
  AnalyzedEvent* m_event;
  mutable TMultiGraph* m_chi2MultiGraph;
  mutable TMultiGraph* m_lhMultiGraph;
};

#endif

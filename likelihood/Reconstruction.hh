#ifndef Reconstruction_hh
#define Reconstruction_hh

#include "Enums.hh"
#include "Hypothesis.hh"

#include <QVector>
#include <QPointF>

class AnalyzedEvent;
class Likelihood;
class TMultiGraph;
class TLegend;

class Reconstruction {
public:
  static Reconstruction* newReconstruction(Enums::ReconstructionMethod, Enums::LikelihoodVariables, Enums::Particles);
  virtual ~Reconstruction();
  Enums::ReconstructionMethod method() const;
  bool externalInformation() const;
  double curvature() const;
  Enums::Particle particle() const;
  int indexOfGlobalMinimum() const;
  const QVector<QPointF>& minima() const;
  const QVector<Enums::Particle>& particles() const;
  virtual void identify(AnalyzedEvent*) = 0;
  virtual TLegend* legend() const;
  virtual TMultiGraph* graph() const;
  double eval(const AnalyzedEvent*, const Hypothesis&, bool* goodInterpolation = 0) const;
protected:
  Reconstruction(Enums::LikelihoodVariables, Enums::Particles);
  void setupDefaultLegend() const;

  static const double s_minimumCurvature;
  static const double s_maximumCurvature;
  bool m_externalInformation;
  Enums::ReconstructionMethod m_method;
  Enums::LikelihoodVariables m_variables;
  QVector<Likelihood*> m_likelihoods;
  QVector<Enums::Particle> m_particles;
  QVector<QPointF> m_minima;
  int m_indexOfGlobalMinimum;
  AnalyzedEvent* m_event;
  mutable TLegend* m_legend;
  mutable TMultiGraph* m_graph;
};

#endif

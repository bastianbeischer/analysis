#ifndef Reconstruction_hh
#define Reconstruction_hh

#include "Enums.hh"

#include <QVector>
#include <QPointF>

class AnalyzedEvent;
class Likelihood;
class TMultiGraph;

class Reconstruction {
public:
  static Reconstruction* newReconstruction(Enums::ReconstructionMethod, Enums::LikelihoodVariables, Enums::Particles);
  virtual ~Reconstruction();
  Enums::ReconstructionMethod method() const;
  double curvature() const;
  Enums::Particle particle() const;
  int indexOfGlobalMinimum() const;
  const QVector<QPointF>& minima() const;
  const QVector<Enums::Particle>& particles() const;
  virtual void identify(AnalyzedEvent*) = 0;
  virtual TMultiGraph* graph() const;
protected:
  Reconstruction(Enums::LikelihoodVariables, Enums::Particles);

  static const double s_minimumCurvature;
  static const double s_maximumCurvature;
  Enums::ReconstructionMethod m_method;
  QVector<Likelihood*> m_likelihoods;
  QVector<Enums::Particle> m_particles;
  QVector<QPointF> m_minima;
  int m_indexOfGlobalMinimum;
  AnalyzedEvent* m_event;
  mutable TMultiGraph* m_graph;
};

#endif

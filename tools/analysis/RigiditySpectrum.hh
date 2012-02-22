#ifndef RigiditySpectrum_hh
#define RigiditySpectrum_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class AnalyzedEvent;

class RigiditySpectrum : public AnalysisPlot, public H1DPlot {
public:
  RigiditySpectrum(Enums::Particles, bool albedo = false);
  ~RigiditySpectrum();
  virtual void processEvent(const AnalyzedEvent*);
private:
  QVector<Enums::Particle> m_particles;
  bool m_albedo;
};

#endif

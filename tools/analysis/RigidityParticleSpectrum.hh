#ifndef RigidityParticleSpectrum_hh
#define RigidityParticleSpectrum_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>

class Hit;
class TH1D;
class Particle;
class SimpleEvent;

class RigidityParticleSpectrum : public AnalysisPlot, public H1DPlot {

public:
  enum Type {NonAlbedo, Albedo};
  RigidityParticleSpectrum(Type, TH1D*);
  ~RigidityParticleSpectrum();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  Type m_type;
};

#endif

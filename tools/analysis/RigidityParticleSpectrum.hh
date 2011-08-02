#ifndef RigidityParticleSpectrum_hh
#define RigidityParticleSpectrum_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <TH1D.h>

#include <QString>

class Hit;
class Particle;
class SimpleEvent;

class RigidityParticleSpectrum : public AnalysisPlot, public H1DPlot {

public:
  enum Type {
    All,
    Positive,
    Negative,
    AlbedoPositive,
    AlbedoNegative,
  };

  RigidityParticleSpectrum(Type type);
  ~RigidityParticleSpectrum();

  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void finalize();
  virtual void update();

  TH1D* particleHistogram() const {return m_particleHistogram;}

private:
  Type m_type;
  TH1D* m_particleHistogram;
};

#endif

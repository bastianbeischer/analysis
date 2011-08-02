#ifndef RigiditySpectrumRatio_hh
#define RigiditySpectrumRatio_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <TH1D.h>

#include <QString>

class Hit;
class Particle;
class SimpleEvent;

class RigiditySpectrumRatio : public AnalysisPlot, public H1DPlot {

public:
  RigiditySpectrumRatio();
  ~RigiditySpectrumRatio();

  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void finalize();
  virtual void update();

private:
  TH1D* m_numerator;
  TH1D* m_denominator;
};

#endif

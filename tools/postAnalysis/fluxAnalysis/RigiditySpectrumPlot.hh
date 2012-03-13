#ifndef RigiditySpectrumPlot_hh
#define RigiditySpectrumPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"
#include "EnumSelector.hh"

#include "TPad.h"

#include <QObject>
#include <QMap>

class TCanvas;
class TH1D;
class EnumsSelector;

class RigiditySpectrumPlot : public QObject, public PostAnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  RigiditySpectrumPlot();
  ~RigiditySpectrumPlot();
  virtual void draw(TCanvas*);
public slots:
  virtual void update();
private:
  typedef EnumSelector<Enums::Particle> ParticleSelector;
  TCanvas* m_canvas;
  ParticleSelector* m_particleSelector;
  ParticleSelector* m_sumSelector;
};

#endif
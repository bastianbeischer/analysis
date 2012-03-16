#ifndef SpectrumPlot_hh
#define SpectrumPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"
#include "EnumSelector.hh"
#include "FluxCalculation.hh"

#include <QObject>

class TCanvas;
class TH1D;
class EnumsSelector;

class SpectrumPlot : public QObject, public PostAnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  virtual ~SpectrumPlot();
  virtual void draw(TCanvas*);
public slots:
  virtual void update();
protected:
  typedef TH1D*(FluxCalculation::*SpectrumFunctionPointer)(Enums::Particle) const;
  typedef EnumSelector<Enums::Particle> ParticleSelector;

  SpectrumPlot(SpectrumFunctionPointer);
  TH1D* newSummedSpectrum(Enums::Particles) const;
  void addSpectra();
  void resizeLegend();

  SpectrumFunctionPointer m_functionPointer;
  TCanvas* m_canvas;
  ParticleSelector* m_particleSelector;
  ParticleSelector* m_sumSelector;
};

#endif

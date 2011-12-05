#ifndef RigidityParticleSpectrumRatio_hh
#define RigidityParticleSpectrumRatio_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QObject>

class TH1D;
class PostAnalysisCanvas;
class TCanvas;
class SimulationFluxRatioWidget;
class BessFluxRatioWidget;

class RigidityParticleSpectrumRatio : public QObject, public PostAnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  RigidityParticleSpectrumRatio(PostAnalysisCanvas* canvas);
  ~RigidityParticleSpectrumRatio();
  virtual void draw(TCanvas* canvas);
  TH1D* ratio() {return histogram();}
private slots:
  void selectionChanged();
private:
  TCanvas* m_canvas;
  SimulationFluxRatioWidget* m_simulationWidget;
  BessFluxRatioWidget* m_bessWidget;
};

#endif

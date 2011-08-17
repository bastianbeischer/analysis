#ifndef RigidityParticleSpectrum_hh
#define RigidityParticleSpectrum_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TH1D;
class PostAnalysisCanvas;

class RigidityParticleSpectrum : public PostAnalysisPlot, public H1DPlot {
public:
  RigidityParticleSpectrum(PostAnalysisCanvas* canvas);
  ~RigidityParticleSpectrum();
  TH1D* spectrum() {return histogram();}
};

#endif

#ifndef RigidityMcSpectrum_hh
#define RigidityMcSpectrum_hh

#include "PostAnalysisPlot.hh"

class PostAnalysisCanvas;
class TH1D;

class RigidityMcSpectrum : public PostAnalysisPlot {
public:
  RigidityMcSpectrum(PostAnalysisCanvas* generated, PostAnalysisCanvas* reconstructed);
  virtual ~RigidityMcSpectrum();
  TH1D* createGeneratedSpectrum();
  TH1D* createRecordedSpectrum();
private:
  void scaleToBinWidth(TH1D*);
  TH1D* m_generated;
  TH1D* m_reconstructed;
};

#endif

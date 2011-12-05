#ifndef RigidityMcSpectrum_hh
#define RigidityMcSpectrum_hh

#include "PostAnalysisPlot.hh"

class PostAnalysisCanvas;
class TH1D;

class RigidityMcSpectrum : public PostAnalysisPlot {
public:
  RigidityMcSpectrum(PostAnalysisCanvas* generated, PostAnalysisCanvas* reconstructed, PostAnalysisCanvas* reconstructedLow, PostAnalysisCanvas* reconstructedUp);
  virtual ~RigidityMcSpectrum();
  TH1D* createGeneratedSpectrum();
  TH1D* createRecordedSpectrum();
  TH1D* createRecordedSpectrumLow();
  TH1D* createRecordedSpectrumUp();

private:
  TH1D* m_generated;
  TH1D* m_reconstructed;
  TH1D* m_reconstructedLow;
  TH1D* m_reconstructedUp;
};

#endif

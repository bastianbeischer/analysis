#ifndef AzimuthDistribution_hh
#define AzimuthDistribution_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TH1D;
class PostAnalysisCanvas;

class AzimuthDistribution : public PostAnalysisPlot, public H1DPlot {
public:
  AzimuthDistribution(PostAnalysisCanvas*);
  virtual ~AzimuthDistribution();
  TH1D* distribution() {return histogram();}
  TH1D* azimuthAcceptance() {return m_azimuthAcceptance;}
private:
  TH1D* m_azimuthAcceptance;
  bool readFile();
  double angleMovement(double azimuth);
};

#endif

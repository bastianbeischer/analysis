#ifndef TimeOfFlightHistogram_hh
#define TimeOfFlightHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TF1;
class TH2D;
class PostAnalysisCanvas;

class TimeOfFlightHistogram : public PostAnalysisPlot, public H1DPlot {
public:
  TimeOfFlightHistogram(PostAnalysisCanvas*, int bin);
  virtual ~TimeOfFlightHistogram();
  double upperY();
  double lowerY();
  double mean();
  double meanError();
  double sigma();
  double sigmaError();
  static TH2D* histgram(TCanvas*);
private:
  double m_upperY;
  double m_lowerY;
  double m_mean;
  double m_meanError;
  double m_sigma;
  double m_sigmaError;
};

#endif

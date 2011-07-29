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
  int upperId() const;
  int lowerId() const;
  double upperY() const;
  double lowerY() const;
  double mean() const;
  double meanError() const;
  double sigma() const;
  double sigmaError() const;
private:
  int m_upperId;
  int m_lowerId;
  double m_upperY;
  double m_lowerY;
  double m_mean;
  double m_meanError;
  double m_sigma;
  double m_sigmaError;
};

#endif

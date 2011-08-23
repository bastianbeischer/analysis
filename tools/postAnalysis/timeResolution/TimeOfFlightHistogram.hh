#ifndef TimeOfFlightHistogram_hh
#define TimeOfFlightHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;
class TH2D;

class TimeOfFlightHistogram : public PostAnalysisPlot, public H1DPlot {
public:
  TimeOfFlightHistogram(PostAnalysisCanvas*, int bin);
  virtual ~TimeOfFlightHistogram();
  int upperId() const;
  int lowerId() const;
  int i() const;
  int j() const;
  int k() const;
  int l() const;
  double upperY() const;
  double lowerY() const;
  double mean() const;
  double meanError() const;
  double sigma() const;
  double sigmaError() const;
  double v() const;
  double vError() const;
  bool fitted() const;
private:
  void extractIndizes(const QString& canvasName, const TH2D* const histogram, int bin);
  void extractParameters();

  int m_upperId;
  int m_lowerId;
  int m_i, m_j, m_k, m_l;
  double m_upperY;
  double m_lowerY;
  double m_mean;
  double m_meanError;
  double m_sigma;
  double m_sigmaError;
  double m_v;
  double m_vError;
  bool m_fitted;
  bool m_fitGood;
};

#endif

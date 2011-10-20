#ifndef TOFProbabilityDensityFunction_hh
#define TOFProbabilityDensityFunction_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TH1D;

class TOFProbabilityDensityFunction : public AnalysisPlot, public H1DPlot {
public:
  TOFProbabilityDensityFunction();
  virtual ~TOFProbabilityDensityFunction();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void update();
  virtual void finalize();
private:
  void addFunctions(double);
  int m_nBins;
  double m_min;
  double m_max;
  TH1D* m_belowThresholdHistogram;
  TH1D* m_aboveThresholdHistogram;
  QVector<double> m_momenta;
};

#endif

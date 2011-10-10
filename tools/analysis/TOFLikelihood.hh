#ifndef TOFLikelihood_hh
#define TOFLikelihood_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TH1D;

class TOFLikelihood : public AnalysisPlot, public H1DPlot {
public:
  TOFLikelihood();
  virtual ~TOFLikelihood();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
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

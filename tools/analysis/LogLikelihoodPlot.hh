#ifndef LogLikelihoodPlot_hh
#define LogLikelihoodPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

#include <QMap>

class LogLikelihoodPlot: public AnalysisPlot, public H1DPlot {
public:
  LogLikelihoodPlot(Enums::Particle signal, Enums::Particles background);
  virtual void processEvent(const AnalyzedEvent*);
  void update();
private:
  Enums::Particle m_signal;
  Enums::Particles m_background;
  int m_nBins;
  TH1D* m_signalHypothesisHistogram;
  TH1D* m_backgroundHypothesisHistogram;
  QMap<Enums::Particle, int> m_typeToIndex;
  QVector<TH1D*> m_backgroundHypothesisHistograms;
};

#endif

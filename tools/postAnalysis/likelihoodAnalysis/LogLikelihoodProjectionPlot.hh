#ifndef LogLikelihoodProjectionPlot_hh
#define LogLikelihoodProjectionPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class LogLikelihoodPlot;

class LogLikelihoodProjectionPlot : public PostAnalysisPlot, public H1DPlot {
public:
  LogLikelihoodProjectionPlot(const LogLikelihoodPlot*, int bin, int numberOfBins = 1);
  ~LogLikelihoodProjectionPlot();
  TH1D* signalProjection() const;
  TH1D* backgroundProjection() const;
private:
  TH1D* m_signalProjection;
  TH1D* m_backgroundProjection;
};

#endif

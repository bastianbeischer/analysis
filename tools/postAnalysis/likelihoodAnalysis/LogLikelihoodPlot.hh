#ifndef LogLikelihoodPlot_hh
#define LogLikelihoodPlot_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class PostAnalysisCanvas;
class TH2D;

class LogLikelihoodPlot : public PostAnalysisPlot, public H2DPlot {
public:
  LogLikelihoodPlot(PostAnalysisCanvas*, PostAnalysisCanvas*);
  ~LogLikelihoodPlot();
  int numberOfBins() const;
  const TH2D* signalHistogram() const;
  const TH2D* backgroundHistogram() const;
private:
  TH2D* m_signalHistogram;
  TH2D* m_backgroundHistogram;
};

#endif

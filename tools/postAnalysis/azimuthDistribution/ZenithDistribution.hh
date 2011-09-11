#ifndef ZenithDistribution_hh
#define ZenithDistribution_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TH1D;
class PostAnalysisCanvas;

class ZenithDistribution : public PostAnalysisPlot, public H1DPlot {
public:
  ZenithDistribution(PostAnalysisCanvas*);
  virtual ~ZenithDistribution();
  TH1D* distribution() {return histogram();}
  TH1D* zenithAcceptance() {return m_zenithAcceptance;}
private:
  TH1D* m_zenithAcceptance;
  bool readFile();
};

#endif

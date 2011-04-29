#ifndef MoPVScaling_hh
#define MoPVScaling_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

class PostAnalysisCanvas;
class TGraphErrors;
class TH2D;

class MoPVScaling : public PostAnalysisPlot, public GraphPlot {
public:
  MoPVScaling(PostAnalysisCanvas*);
  virtual ~MoPVScaling();
  
private:
  TGraphErrors* m_mopvGraph;
  TGraphErrors* generateMoPVGraph(TH2D* histogram);

};

#endif

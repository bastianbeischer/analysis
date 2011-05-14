#ifndef MoPVScaling_hh
#define MoPVScaling_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QList>

class PostAnalysisCanvas;
class TMultiGraph;
class TGraphErrors;
class TH2D;

class MoPVScaling : public PostAnalysisPlot, public GraphPlot {
public:
  MoPVScaling(PostAnalysisCanvas*);
  MoPVScaling(QList<PostAnalysisCanvas*>);
  virtual ~MoPVScaling();
  
private:
  QList<TGraphErrors*> m_mopvGraphs;
  TGraphErrors* generateMoPVGraph(TH2D* histogram);

  int m_colorCounter;

};

#endif

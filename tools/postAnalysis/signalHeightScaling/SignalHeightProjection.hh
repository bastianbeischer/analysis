#ifndef SignalHeightProjection_hh
#define SignalHeightProjection_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;

class SignalHeightProjection : public PostAnalysisPlot, public H1DPlot {
public:
  SignalHeightProjection(PostAnalysisCanvas*, unsigned short id);
  virtual ~SignalHeightProjection();
private:
  unsigned short m_id;
};

#endif

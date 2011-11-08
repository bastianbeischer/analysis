#ifndef SignalHeightTempCor_hh
#define SignalHeightTempCor_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <TGraphErrors.h>

class SignalHeightTempCor : public PostAnalysisPlot, public GraphPlot {
public:
  SignalHeightTempCor(PostAnalysisCanvas*, unsigned short sipmId);
  virtual ~SignalHeightTempCor();
private:
  unsigned short m_sipmId;
  TGraphErrors* meanGraph(TH2D* histogram);
};

#endif

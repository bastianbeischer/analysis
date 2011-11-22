#ifndef SignalHeightTempCorrelation_hh
#define SignalHeightTempCorrelation_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <TGraphErrors.h>

class SignalHeightTempCorrelation : public PostAnalysisPlot, public GraphPlot {
public:
  SignalHeightTempCorrelation(PostAnalysisCanvas*, unsigned short sipmId);
  virtual ~SignalHeightTempCorrelation();
private:
  unsigned short m_sipmId;
  TGraphErrors* meanGraph(TH2D* histogram);
};

#endif

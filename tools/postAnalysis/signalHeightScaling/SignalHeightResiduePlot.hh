#ifndef SignalHeightResiduePlot_hh
#define SignalHeightResiduePlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class PostAnalysisCanvas;
class TFile;

class SignalHeightResiduePlot : public PostAnalysisPlot, public H1DPlot {
public:
  enum Type {Time, Temperature};
  SignalHeightResiduePlot(Type type, TFile* file);
  virtual ~SignalHeightResiduePlot();
private:
  const static double s_reference;
};

#endif

#ifndef ReconstructionMethodCorrelationPlot_hh
#define ReconstructionMethodCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TGraphErrors;

class ReconstructionMethodCorrelationPlot : public AnalysisPlot, public H2DPlot {
public:
  explicit ReconstructionMethodCorrelationPlot(Enums::ReconstructionMethod, Enums::ReconstructionMethod);
  virtual ~ReconstructionMethodCorrelationPlot();
  virtual void processEvent(const AnalyzedEvent*);
private:
  Enums::ReconstructionMethod m_methodX;
  Enums::ReconstructionMethod m_methodY;
};

#endif

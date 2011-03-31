#ifndef BarTimeShiftHistogram_hh
#define BarTimeShiftHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

#include "PostAnalysisCanvas.hh"

class BarTimeShiftHistogram : public PostAnalysisPlot, public H2DPlot {
public:
  BarTimeShiftHistogram(const QVector<PostAnalysisCanvas*>&);
  virtual ~BarTimeShiftHistogram();
};

#endif

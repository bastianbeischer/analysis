#ifndef BarTimeShiftHistogram_hh
#define BarTimeShiftHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class BarTimeShiftHistogram : public PostAnalysisPlot, public H2DPlot {
public:
  BarTimeShiftHistogram(const QVector<TCanvas*>&);
  virtual ~BarTimeShiftHistogram();
	virtual void draw(TCanvas*);
};

#endif

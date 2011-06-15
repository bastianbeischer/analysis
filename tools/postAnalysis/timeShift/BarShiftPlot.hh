#ifndef BarShiftPlot_hh
#define BarShiftPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <TMatrixT.h>

#include <QVector>

class PostAnalysisCanvas;
class TLine;

class BarShiftPlot : public PostAnalysisPlot, public H1DPlot {
public:
  BarShiftPlot(PostAnalysisCanvas*);
  virtual ~BarShiftPlot();
  virtual void draw(TCanvas*);
private:
  void dumpMatrix(const TMatrixT<double>&);
  QVector<TLine*> m_lines;
};

#endif

#ifndef BarShiftPlot_hh
#define BarShiftPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QVector>

class PostAnalysisCanvas;
class TLine;

class BarShiftPlot : public PostAnalysisPlot, public H1DPlot {
public:
  BarShiftPlot(PostAnalysisCanvas*);
  virtual ~BarShiftPlot();
  double dt() const {return m_dt;}
  double errDt() const {return m_errDt;}
  virtual void draw(TCanvas*);
private:
  double m_dt;
  double m_errDt;
  QVector<TLine*> m_lines;
};

#endif

#ifndef PostAnalysisH2DPlot_hh
#define PostAnalysisH2DPlot_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QVector>

class TH2D;

class PostAnalysisH2DPlot : public PostAnalysisPlot, public H2DPlot {
public:
  PostAnalysisH2DPlot(TH2D*);
  PostAnalysisH2DPlot(const QVector<TH2D*>&);
  virtual ~PostAnalysisH2DPlot();
private:
  void init(const QVector<TH2D*>&);
};

#endif

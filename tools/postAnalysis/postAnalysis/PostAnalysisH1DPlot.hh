#ifndef PostAnalysisH1DPlot_hh
#define PostAnalysisH1DPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>
#include <QVector>

#include <TH1D.h>

class PostAnalysisH1DPlot : public PostAnalysisPlot, public H1DPlot {
public:
  PostAnalysisH1DPlot(TH1D* h1);
  PostAnalysisH1DPlot(const QVector<TH1D*>&);
  virtual ~PostAnalysisH1DPlot();
private:
  void init(const QVector<TH1D*>&);
};

#endif

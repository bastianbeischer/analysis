#ifndef PostAnalysisGraphPlot_hh
#define PostAnalysisGraphPlot_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QVector>

class TGraph;

class PostAnalysisGraphPlot : public PostAnalysisPlot, public GraphPlot {
public:
  PostAnalysisGraphPlot(TGraph*);
  PostAnalysisGraphPlot(const QVector<TGraph*>&);
  virtual ~PostAnalysisGraphPlot();
private:
  void init(const QVector<TGraph*>&);
};

#endif

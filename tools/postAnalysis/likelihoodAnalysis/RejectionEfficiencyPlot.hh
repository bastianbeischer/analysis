#ifndef RejectionEfficiencyPlot_hh
#define RejectionEfficiencyPlot_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QVector>

class LogLikelihoodProjectionPlot;
class TLine;

class RejectionEfficiencyPlot : public PostAnalysisPlot, public GraphPlot {
public:
  RejectionEfficiencyPlot(LogLikelihoodProjectionPlot*);
  ~RejectionEfficiencyPlot();
  void draw(TCanvas* canvas);
private:
  QVector<TLine*> m_verticalLines;
  QVector<TLine*> m_horizontalLines;
};

#endif

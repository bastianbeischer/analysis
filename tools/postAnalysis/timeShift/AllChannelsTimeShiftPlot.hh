#ifndef AllChannelsTimeShiftPlot_hh
#define AllChannelsTimeShiftPlot_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QVector>

class PostAnalysisCanvas;
class TLine;

class AllChannelsTimeShiftPlot : public PostAnalysisPlot, public H2DPlot {
public:
  AllChannelsTimeShiftPlot(const QVector<PostAnalysisCanvas*>&, int ch);
  virtual ~AllChannelsTimeShiftPlot();
private:
};

#endif

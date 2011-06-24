#ifndef HISTRESIDUALS_HH
#define HISTRESIDUALS_HH

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "GraphPlot.hh"

#include <QObject>

class PostAnalysisCanvas;
class TH1D;

class HistResiduals: public QObject, public PostAnalysisPlot, public GraphPlot
{
  Q_OBJECT
public:
  HistResiduals(PostAnalysisCanvas* dataC, PostAnalysisCanvas* mcC);

};

#endif // HISTRESIDUALS_HH

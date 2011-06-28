#ifndef LHFIT_HH
#define LHFIT_HH

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QObject>
#include <QList>

class PostAnalysisCanvas;
class TH1D;

class LHFit: public QObject, public PostAnalysisPlot, public H1DPlot
{
  Q_OBJECT
public:
  LHFit(PostAnalysisCanvas* canvases);

};

#endif // LHFIT_HH

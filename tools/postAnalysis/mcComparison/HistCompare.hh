#ifndef HISTCOMPARE_HH
#define HISTCOMPARE_HH

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QObject>

class PostAnalysisCanvas;
class TH1D;

class HistCompare: public QObject, public PostAnalysisPlot, public H1DPlot
{
  Q_OBJECT
public:
  HistCompare(PostAnalysisCanvas* dataC, PostAnalysisCanvas* mcC, PostAnalysisCanvas* oldmcC = 0);

};

#endif // HISTCOMPARE_HH

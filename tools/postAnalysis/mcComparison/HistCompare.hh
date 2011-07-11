#ifndef HISTCOMPARE_HH
#define HISTCOMPARE_HH

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QObject>
#include <QList>

class PostAnalysisCanvas;
class TH1D;

class HistCompare: public QObject, public PostAnalysisPlot, public H1DPlot
{
  Q_OBJECT
public:
  HistCompare(QList<PostAnalysisCanvas*> canvases);

};

#endif // HISTCOMPARE_HH

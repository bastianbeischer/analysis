#ifndef TRDLIKELIHOODFUNCTIONSPLOT_HH
#define TRDLIKELIHOODFUNCTIONSPLOT_HH

class Particle;
class SimpleEvent;
class Hit;

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QVector>


class TRDLikelihoodFunctionsPlot: public AnalysisPlot, public H1DPlot
{
public:
    TRDLikelihoodFunctionsPlot();
    ~TRDLikelihoodFunctionsPlot();
    virtual void processEvent(const AnalyzedEvent*);
};

#endif // TRDLIKELIHOODFUNCTIONSPLOT_HH

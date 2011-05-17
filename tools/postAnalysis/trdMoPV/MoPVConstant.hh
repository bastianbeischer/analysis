#ifndef MOPVCONSTANT_HH
#define MOPVCONSTANT_HH

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QList>

class PostAnalysisCanvas;
class TH1D;

class MoPVConstant : public PostAnalysisPlot, public H1DPlot
{
public:
    MoPVConstant(QList<PostAnalysisCanvas*>);
private:
    bool fitMoPV(TH1D* hist, double& mopv, double& mopvErr);
};

#endif // MOPVCONSTANT_HH

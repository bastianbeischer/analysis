#ifndef MCRIGIDITYRESOLUTION_HH
#define MCRIGIDITYRESOLUTION_HH

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class MCRigidityResolution : public AnalysisPlot, public H1DPlot
{
public:
    MCRigidityResolution();

     virtual void processEvent(const QVector<Hit*>&, Track*, SimpleEvent*);
     virtual void update();
     virtual void finalize();
};

#endif // MCRIGIDITYRESOLUTION_HH

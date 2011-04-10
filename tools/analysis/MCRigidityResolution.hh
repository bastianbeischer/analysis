#ifndef MCRIGIDITYRESOLUTION_HH
#define MCRIGIDITYRESOLUTION_HH

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QMap>
#include <QPair>

class MCRigidityResolution : public AnalysisPlot, public H1DPlot
{
public:
    MCRigidityResolution();

     virtual void processEvent(const QVector<Hit*>&, Track*, SimpleEvent*);
     virtual void update();
     virtual void finalize();
private:
     const double m_rigidityRangeLower;
     const double m_rigidityRangeUppper;
     const int m_numberOfBins;

    QMap < int, TH1D*> m_resolutionHistos;
};

#endif // MCRIGIDITYRESOLUTION_HH

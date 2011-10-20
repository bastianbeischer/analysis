#ifndef MCTRDCALIBRATIONPLOT_HH
#define MCTRDCALIBRATIONPLOT_HH

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class MCTRDCalibrationPlot : public AnalysisPlot, public H2DPlot {
public:
    MCTRDCalibrationPlot();
    virtual void processEvent(const AnalyzedEvent*);
    //virtual void finalize();
};

#endif // MCTRDCALIBRATIONPLOT_HH

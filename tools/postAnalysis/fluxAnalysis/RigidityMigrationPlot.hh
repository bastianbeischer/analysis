#ifndef RigidityMigrationPlot_hh
#define RigidityMigrationPlot_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class PostAnalysisCanvas;
class TH2D;
class TH1D;

class RigidityMigrationPlot : public PostAnalysisPlot, public H2DPlot {
public:
  RigidityMigrationPlot(PostAnalysisCanvas*);
  virtual ~RigidityMigrationPlot();
  TH2D* migrationHistogram() {return histogram();}
  TH2D* createMigrationHistogramNormalizedToArea();
  TH1D* createGeneratedSpectrum();
  TH1D* createRecordedSpectrum();
private:
  TH1D* projectionX(TH2D*, int firstBin, int lastBin);
  TH1D* projectionY(TH2D*, int firstBin, int lastBin);
};

#endif

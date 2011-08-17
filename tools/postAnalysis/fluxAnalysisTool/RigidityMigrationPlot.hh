#ifndef RigidityMigrationPlot_hh
#define RigidityMigrationPlot_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class PostAnalysisCanvas;
class TH2D;

class RigidityMigrationPlot : public PostAnalysisPlot, public H2DPlot {
public:
  RigidityMigrationPlot(PostAnalysisCanvas*);
  virtual ~RigidityMigrationPlot();
  TH2D* migrationHistogram() {return histogram();}
};

#endif

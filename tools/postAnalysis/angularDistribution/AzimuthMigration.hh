#ifndef AzimuthMigration_hh
#define AzimuthMigration_hh

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class TH2D;
class PostAnalysisCanvas;

class AzimuthMigration : public PostAnalysisPlot, public H2DPlot {
public:
  AzimuthMigration(PostAnalysisCanvas*);
  virtual ~AzimuthMigration();
  TH2D* migrationHistogram() {return histogram();}
};

#endif

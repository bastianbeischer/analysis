#ifndef AzimuthMigration_hh
#define AzimuthMigration_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QString>

#include <TH2D.h>

class AzimuthMigration : public PostAnalysisPlot, public H2DPlot {
public:
  AzimuthMigration(PostAnalysisCanvas*);
  virtual ~AzimuthMigration();
  TH2D* migrationHistogram() {return histogram();}

private:


};

#endif

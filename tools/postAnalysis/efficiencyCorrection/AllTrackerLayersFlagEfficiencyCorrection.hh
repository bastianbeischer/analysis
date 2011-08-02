#ifndef AllTrackerLayersFlagEfficiencyCorrection_hh
#define AllTrackerLayersFlagEfficiencyCorrection_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>
#include <QVector>
#include <QMap>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1D.h>

class AllTrackerLayersFlagEfficiencyCorrection : public PostAnalysisPlot, public H1DPlot {
public:
  AllTrackerLayersFlagEfficiencyCorrection(PostAnalysisCanvas*);
  virtual ~AllTrackerLayersFlagEfficiencyCorrection();
private:
  QString m_name;
  void saveAsSetting();
};

#endif

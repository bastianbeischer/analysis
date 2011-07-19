#ifndef OneHitAllLayersEfficiencyCorrection_hh
#define OneHitAllLayersEfficiencyCorrection_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>
#include <QVector>
#include <QMap>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1D.h>

class OneHitAllLayersEfficiencyCorrection : public PostAnalysisPlot, public H1DPlot {
public:
  OneHitAllLayersEfficiencyCorrection(PostAnalysisCanvas*, QString config);
  virtual ~OneHitAllLayersEfficiencyCorrection();
  
public:
  
private:
  QString m_name;
  
  void saveAsSetting(QString config);

};

#endif

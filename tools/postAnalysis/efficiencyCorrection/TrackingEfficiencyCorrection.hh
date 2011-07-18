#ifndef TrackingEfficiencyCorrection_hh
#define TrackingEfficiencyCorrection_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>
#include <QVector>
#include <QMap>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1D.h>

class TrackingEfficiencyCorrection : public PostAnalysisPlot, public H1DPlot {
public:
  TrackingEfficiencyCorrection(PostAnalysisCanvas*);
  virtual ~TrackingEfficiencyCorrection();
  
public:
  
private:
  TF1* m_efficiencyFit;
  
  void fit();
  void saveAsSetting();

};

#endif

#ifndef TrackFindingEfficiencyCorrection_hh
#define TrackFindingEfficiencyCorrection_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>
#include <QVector>
#include <QMap>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1D.h>

class TrackFindingEfficiencyCorrection : public PostAnalysisPlot, public H1DPlot {
public:
  TrackFindingEfficiencyCorrection(PostAnalysisCanvas*);
  virtual ~TrackFindingEfficiencyCorrection();
  
public:
  
private:
  QString m_name;
  
  void saveAsSetting();

};

#endif

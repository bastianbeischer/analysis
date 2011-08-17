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
  enum Type {
    Positive,
    Negative
  };
  AllTrackerLayersFlagEfficiencyCorrection(Type, PostAnalysisCanvas*);
  virtual ~AllTrackerLayersFlagEfficiencyCorrection();
private:
  Type m_type;
  void saveAsSetting();
  QMap<Type, QString> m_typeNames;
};

#endif

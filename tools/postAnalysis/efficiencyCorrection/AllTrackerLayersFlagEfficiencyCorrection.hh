#ifndef AllTrackerLayersFlagEfficiencyCorrection_hh
#define AllTrackerLayersFlagEfficiencyCorrection_hh

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

#include <QObject>
#include <QString>
#include <QMap>

class AllTrackerLayersFlagEfficiencyCorrection : public QObject, public PostAnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  AllTrackerLayersFlagEfficiencyCorrection(Enums::ChargeSign, PostAnalysisCanvas*);
  virtual ~AllTrackerLayersFlagEfficiencyCorrection();
private slots:
  void save();
private:
  Enums::ChargeSign m_chargeSign;
  EfficiencyCorrectionSettings::FoldingType m_foldingType;
};

#endif

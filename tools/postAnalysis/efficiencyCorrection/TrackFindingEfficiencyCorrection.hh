#ifndef TrackFindingEfficiencyCorrection_hh
#define TrackFindingEfficiencyCorrection_hh

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

#include <QObject>
#include <QString>
#include <QMap>

class TrackFindingEfficiencyCorrection : public QObject, public PostAnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  TrackFindingEfficiencyCorrection(Enums::ChargeSign, PostAnalysisCanvas*);
  virtual ~TrackFindingEfficiencyCorrection();
private slots:
  void save();
private:
  Enums::ChargeSign m_chargeSign;
  EfficiencyCorrectionSettings::FoldingType m_foldingType;
};

#endif

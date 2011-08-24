#ifndef EfficiencyCorrectionSettings_hh
#define EfficiencyCorrectionSettings_hh

#include <QString>
#include <QSettings>
#include <QMap>

class TH1D;

class EfficiencyCorrectionSettings {
public:
  enum BinQuantity {Raw, Unfolded, Undefined};
  static EfficiencyCorrectionSettings* instance();
  ~EfficiencyCorrectionSettings();
  void saveTrackFindingEfficiency(BinQuantity quantity, TH1D*);
  void saveAllTrackerLayerCutEfficiency(BinQuantity quantity, TH1D*);
  TH1D* trackFindingEfficiency(BinQuantity quantity);
  TH1D* allTrackerLayerCutEfficiency(BinQuantity quantity);
  BinQuantity binQuantity(int nBins) const;
  QString binQuantityName(int nBins) const;
  QString binQuantityName(BinQuantity quantity) const;
  static void efficiencyCorrection(TH1D* histogramToCorrect, double efficiency);
  static void efficiencyCorrection(TH1D* histogramToCorrect, TH1D* efficiencyHistogram);
private:
  EfficiencyCorrectionSettings();
  TH1D* readHistogram(const QString& key);
  void save(const QString& key, TH1D*);
  static EfficiencyCorrectionSettings* s_instance;
  QMap<BinQuantity, TH1D*> m_trackFindingEfficiency;
  QMap<BinQuantity, TH1D*> m_allTrackerLayerCutEfficiency;
  QMap<BinQuantity, int> m_binQuantities;
  QSettings m_settings;
  QString m_trackFindingKey;
  QString m_allTrackerLayerCutKey;
  QString m_axisKey;
  QString m_contentKey;
  QString m_errorKey;
};

#endif

#ifndef EfficiencyCorrectionSettings_hh
#define EfficiencyCorrectionSettings_hh

#include <QString>
#include <QSettings>

class TH1D;

class EfficiencyCorrectionSettings {
public:
  static EfficiencyCorrectionSettings* instance();
  ~EfficiencyCorrectionSettings();
  void saveTrackFindingEfficiency(TH1D*);
  void saveAllTrackerLayerCutEfficiency(TH1D*);
  TH1D* trackFindingEfficiency();
  TH1D* allTrackerLayerCutEfficiency();
  static void efficiencyCorrection(TH1D* histogramToCorrect, double efficiency);
  static void efficiencyCorrection(TH1D* histogramToCorrect, TH1D* efficiencyHistogram);
private:
  EfficiencyCorrectionSettings();
  TH1D* readHistogram(const QString& key);
  void save(const QString& key, TH1D*);

  static EfficiencyCorrectionSettings* s_instance;
  TH1D* m_trackFindingEfficiency;
  TH1D* m_allTrackerLayerCutEfficiency;
  QSettings m_settings;
  QString m_trackFindingKey;
  QString m_allTrackerLayerCutKey;
  QString m_axisKey;
  QString m_contentKey;
  QString m_errorKey;
};

#endif

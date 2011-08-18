#ifndef EfficiencyCorrectionSettings_hh
#define EfficiencyCorrectionSettings_hh

#include <QSettings>
#include <QString>
#include <QMap>

class TH1D;

class EfficiencyCorrectionSettings {
public:
  static EfficiencyCorrectionSettings* instance();
	~EfficiencyCorrectionSettings();
  void saveTrackFindingEfficiency(TH1D* histogram);
  void saveAllTrackerLayerCutEfficiency(TH1D* histogram);
  TH1D* trackFindingEfficiency();
  TH1D* allTrackerLayerCutEfficiency();
private:
  EfficiencyCorrectionSettings();
  void loadEfficiencies();
  TH1D* readHistogram(const QString& key);
  void save(const QString& key, TH1D* histogram);
  static EfficiencyCorrectionSettings* s_instance;
  bool m_efficienciesLoaded;
  QString m_trackFindingKey;
  QString m_allTrackerLayerCutKey;
  TH1D* m_trackFindingEfficiency;
  TH1D* m_allTrackerLayerCutEfficiency;
  QSettings* m_settings;
  QString m_axisKey;
  QString m_contentKey;
  QString m_errorKey;
};

#endif

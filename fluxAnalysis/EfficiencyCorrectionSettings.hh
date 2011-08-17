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
  void save(const QString& key, TH1D* histogram);
  TH1D* histogram(const QString& key);
  static QString s_trackFindingEfficiencyPreKey;
  static QString s_allTrackerLayerCutEfficiencyPreKey;

private:
  EfficiencyCorrectionSettings();
  void loadEfficiencies();
  TH1D* readHistogram(const QString& key);
  bool m_efficienciesLoaded;
  static EfficiencyCorrectionSettings* s_instance;
  QMap<QString, TH1D*> m_histograms;
  QSettings* m_settings;
  QString m_axisKey;
  QString m_contentKey;
  QString m_errorKey;
};


#endif

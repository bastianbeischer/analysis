#ifndef EfficiencyCorrectionSettings_hh
#define EfficiencyCorrectionSettings_hh

#include <QSettings>
#include <QString>

class TH1D;

class EfficiencyCorrectionSettings {

public:
  EfficiencyCorrectionSettings();
	~EfficiencyCorrectionSettings();
  void save(const QString& key, TH1D* histogram);
  TH1D* readHistogram(const QString& key);
  static QString s_trackFindingEfficiencyPreKey;
  static QString s_allTrackerLayerCutEfficiencyPreKey;

private:
  QSettings* m_settings;
  QString m_axisKey;
  QString m_contentKey;
  QString m_errorKey;

};


#endif

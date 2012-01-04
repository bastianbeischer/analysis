#ifndef SettingsManager_hh
#define SettingsManager_hh

#include <QVector>

class QSettings;
class Settings;
class SimpleEvent;

class SettingsManager
{

public:
  ~SettingsManager();

  static SettingsManager* instance();
  const Settings* settingsForEvent(const SimpleEvent* const) const;
  const Settings* settingsForRun(int) const;

private:
  SettingsManager();
  void readSettings();

private:
  static SettingsManager* s_instance;
  QSettings* m_configFile;
  QVector<const Settings*> m_settings;

};

#endif /* SettingsManager_hh */

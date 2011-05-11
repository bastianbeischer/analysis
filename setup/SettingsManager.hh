#ifndef SettingsManager_hh
#define SettingsManager_hh

#include <QMap>
#include <QPair>

class QSettings;
class Settings;
class SimpleEvent;

class SettingsManager
{
  
public:
  ~SettingsManager();
  
  static SettingsManager* instance();
  const Settings* settingsForEvent(SimpleEvent*) const;
  const Settings* settingsForRun(int) const;

private:
  SettingsManager();
  void readSettings();

private:
  static SettingsManager* s_instance;
  QSettings* m_configFile;
  QList<const Settings*> m_settings;

};

#endif /* SettingsManager_hh */

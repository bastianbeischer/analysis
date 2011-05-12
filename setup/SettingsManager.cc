#include "SettingsManager.hh"

#include <QSettings>
#include <QStringList>
#include <QDebug>

#include <cstdlib>

#include "Settings.hh"
#include "SimpleEvent.hh"

SettingsManager* SettingsManager::s_instance = 0;

SettingsManager::SettingsManager() :
  m_configFile(0)
{
  char* env = getenv("PERDAIXANA_PATH");
  if (env == 0) {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }
  QString path(env);
  path += "/conf/";
  m_configFile = new QSettings(path+"settings.conf", QSettings::IniFormat);

  readSettings();
}

SettingsManager::~SettingsManager()
{
  delete m_configFile;
  qDeleteAll(m_settings);
}

SettingsManager* SettingsManager::instance()
{
  if (!s_instance) s_instance = new SettingsManager;
  return s_instance;
}

const Settings* SettingsManager::settingsForEvent(SimpleEvent* event) const
{
  int run = event->runStartTime();
  return settingsForRun(run);
}

const Settings* SettingsManager::settingsForRun(int run) const
{
  foreach(const Settings* settings, m_settings) {
    int firstRun = settings->firstRun();
    int lastRun = settings->lastRun();
    if (run >= firstRun && run <= lastRun) {
      return settings;
    }
  }
  return 0;
}

void SettingsManager::readSettings()
{
  foreach(QString group, m_configFile->childGroups()) {
    m_configFile->beginGroup(group);

    int identifier = group.toInt();
    QString name = m_configFile->value("name").toString();
    int firstRun = m_configFile->value("first_run").toInt();
    int lastRun = m_configFile->value("last_run").toInt();
    double momentum = m_configFile->value("momentum").toDouble();
    bool magnet = m_configFile->value("magnet").toString() == "yes" ? true : false;
    QString situationString = m_configFile->value("situation").toString().toLower();
    Settings::Situation situation;
    if (situationString == "unknown") situation = Settings::Unknown;
    else if (situationString == "kirunamuons") situation = Settings::KirunaMuons;
    else if (situationString == "kirunafloat") situation = Settings::KirunaFloat;
    else if (situationString == "testbeam11") situation = Settings::Testbeam11;

    Settings* settings = new Settings;
    settings->setIdentifier(identifier);
    settings->setName(name);
    settings->setFirstRun(firstRun);
    settings->setLastRun(lastRun);
    settings->setMomentum(momentum);
    settings->setMagnet(magnet);
    settings->setSituation(situation);
    
    m_settings.append(settings);
    m_configFile->endGroup();
  }
}

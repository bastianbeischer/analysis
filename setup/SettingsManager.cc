#include "SettingsManager.hh"

#include <QSettings>
#include <QStringList>
#include <QDebug>
#include <QDir>

#include <cstdlib>

#include "Settings.hh"
#include "SimpleEvent.hh"
#include "Helpers.hh"

SettingsManager* SettingsManager::s_instance = 0;

bool settingsCompare(const Settings* s1, const Settings* s2)
{
  return s1->lastRun() < s2->firstRun();
}

SettingsManager::SettingsManager() :
  m_configFile(0)
{
  QString path = Helpers::analysisPath() + "/conf/";
  QDir dir(path);
  if (!dir.exists("settings.conf")) {
    qFatal("ERROR: settings.conf not found!");
  }
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

const Settings* SettingsManager::settingsForEvent(const SimpleEvent* const event) const
{
  int run = event->runStartTime();
  return settingsForRun(run);
}

const Settings* SettingsManager::settingsForRun(int run) const
{
  Settings temp;
  temp.setFirstRun(run);
  temp.setLastRun(run);
  const QVector<const Settings*>::const_iterator it = qBinaryFind(m_settings.begin(), m_settings.end(), &temp, &settingsCompare);
  if (it != m_settings.constEnd())
    return *it;
  return 0;
}

void SettingsManager::readSettings()
{
  foreach(QString group, m_configFile->childGroups()) {
    m_configFile->beginGroup(group);

    int identifier = group.toInt();
    QString name = m_configFile->value("name").toString();
    QString situationString = m_configFile->value("situation", "unknown").toString().toLower();
    int firstRun = m_configFile->value("first_run").toInt();
    int lastRun = m_configFile->value("last_run").toInt();
    int polarity = m_configFile->value("polarity", 0).toString() == "positive" ? 1 : -1;
    double pressure = m_configFile->value("pressure", 0.).toDouble();
    double momentum = m_configFile->value("momentum", 0.).toDouble();
    bool magnet = m_configFile->value("magnet", "yes").toString() == "yes" ? true : false;

    Settings::Situation situation;
    if (situationString == "unknown") situation = Settings::Unknown;
    else if (situationString == "kirunamuons") situation = Settings::KirunaMuons;
    else if (situationString == "kirunaflight") situation = Settings::KirunaFlight;
    else if (situationString == "testbeam11") situation = Settings::Testbeam11;

    Q_ASSERT(firstRun <= lastRun);

    Settings* settings = new Settings;
    settings->setIdentifier(identifier);
    settings->setName(name);
    settings->setFirstRun(firstRun);
    settings->setLastRun(lastRun);
    settings->setMomentum(momentum);
    settings->setPressure(pressure);
    settings->setMagnet(magnet);
    settings->setPolarity(polarity);
    settings->setSituation(situation);
    
    m_settings.append(settings);
    m_configFile->endGroup();
  }
  qSort(m_settings.begin(), m_settings.end(), &settingsCompare);
}

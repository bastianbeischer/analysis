#include "AnalysisSetting.hh"

#include <QSettings>
#include <QDebug>

AnalysisSetting::AnalysisSetting()
{
  clear();
}

void AnalysisSetting::clear()
{
  m_firstEvent = -1;
  m_lastEvent = -1;
  m_numberOfThreads = -1;
  m_analysisTopics = Enums::NoTopic;
  AnalysisProcessorSetting::clear();
}

void AnalysisSetting::save(const QString& file) const
{
  QSettings settings(file, QSettings::IniFormat);
  settings.beginGroup("AnalysisSetting");
  settings.setValue("firstEvent", m_firstEvent);
  settings.setValue("lastEvent", m_lastEvent);
  settings.setValue("numberOfThreads", m_numberOfThreads);
  settings.setValue("analysisTopics", Enums::label(m_analysisTopics));
  settings.endGroup();
  settings.sync();
  AnalysisProcessorSetting::save(file);
}

void AnalysisSetting::load(const QString& file)
{
  clear();
  QSettings settings(file, QSettings::IniFormat);
  settings.beginGroup("AnalysisSetting");
  m_firstEvent = settings.value("firstEvent").toInt();
  m_lastEvent = settings.value("lastEvent").toInt();
  m_numberOfThreads = settings.value("numberOfThreads").toInt();
  m_analysisTopics = Enums::analysisTopics(settings.value("analysisTopics").toString());
  settings.endGroup();
  AnalysisProcessorSetting::load(file);
}

QDebug operator<<(QDebug debug, const AnalysisSetting& setting)
{
  debug
    << static_cast<const AnalysisProcessorSetting&>(setting)
    << "\nAnalysisSetting: firstEvent =" << setting.firstEvent() << ", lastEvent =" << setting.lastEvent()
    << ", numberOfThreads =" << setting.numberOfThreads() << qPrintable(", analysisTopics = " + Enums::label(setting.analysisTopics()));
  return debug;
}

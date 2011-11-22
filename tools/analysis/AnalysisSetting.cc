#include "AnalysisSetting.hh"

#include <QSettings>

AnalysisSetting::AnalysisSetting()
{
  clear();
}

void AnalysisSetting::clear()
{
  firstEvent = -1;
  lastEvent = -1;
  numberOfThreads = -1;
  analysisTopics = Enums::NoTopic;
  trackType = Enums::NoTrack;
  reconstructionMethod = Enums::UndefinedReconstructionMethod;
  corrections = Enums::NoCorrection;
  particles = Enums::NoParticle;
  particleFilter = Enums::NoParticle;
  mcParticleFilter = Enums::NoParticle;
  cutFilter.clear();
}

void AnalysisSetting::save(const QString& file) const
{
  QSettings settings(file, QSettings::IniFormat);
  settings.beginGroup("AnalysisSetting");
  settings.setValue("firstEvent", firstEvent);
  settings.setValue("lastEvent", lastEvent);
  settings.setValue("numberOfThreads", numberOfThreads);
  settings.setValue("analysisTopics", Enums::label(analysisTopics));
  settings.setValue("trackType", Enums::label(trackType));
  settings.setValue("reconstructionMethod", Enums::label(reconstructionMethod));
  settings.setValue("corrections", Enums::label(corrections));
  settings.setValue("particles", Enums::label(particles));
  settings.setValue("particleFilter", Enums::label(particleFilter));
  settings.setValue("mcParticleFilter", Enums::label(mcParticleFilter));
  settings.setValue("cuts", cutFilter.toString());
  settings.endGroup();
}

void AnalysisSetting::load(const QString& file)
{
  clear();
  QSettings settings(file, QSettings::IniFormat);
  settings.beginGroup("AnalysisSetting");
  firstEvent = settings.value("firstEvent").toInt();
  lastEvent = settings.value("lastEvent").toInt();
  numberOfThreads = settings.value("numberOfThreads").toInt();
  analysisTopics = Enums::analysisTopics(settings.value("analysisTopics").toString());
  trackType = Enums::trackType(settings.value("trackType").toString());
  reconstructionMethod = Enums::reconstructionMethod(settings.value("reconstructionMethod").toString());
  corrections = Enums::corrections(settings.value("corrections").toString());
  particles = Enums::particles(settings.value("particles").toString());
  particleFilter = Enums::particles(settings.value("particleFilter").toString());
  mcParticleFilter = Enums::particles(settings.value("mcParticleFilter").toString());
  cutFilter = CutFilter(settings.value("cuts").toString());
  settings.endGroup();
}

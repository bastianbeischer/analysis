#include "AnalysisProcessorSetting.hh"

#include <QSettings>

AnalysisProcessorSetting::AnalysisProcessorSetting()
{
  clear();
}

void AnalysisProcessorSetting::clear()
{
  m_trackType = Enums::NoTrack;
  m_reconstructionMethods = Enums::UndefinedReconstructionMethod;
  m_reconstructionMethod = Enums::UndefinedReconstructionMethod;
  m_corrections = Enums::NoCorrection;
  m_particles = Enums::NoParticle;
  m_likelihoods = Enums::UndefinedLikelihood;
  m_particleFilter = Enums::NoParticle;
  m_mcParticleFilter = Enums::NoParticle;
  m_cutFilter.clear();
}

void AnalysisProcessorSetting::save(const QString& file) const
{
  QSettings settings(file, QSettings::IniFormat);
  settings.beginGroup("AnalysisProcessorSetting");
  settings.setValue("trackType", Enums::label(m_trackType));
  settings.setValue("reconstructionMethods", Enums::label(m_reconstructionMethods));
  settings.setValue("reconstructionMethod", Enums::label(m_reconstructionMethod));
  settings.setValue("corrections", Enums::label(m_corrections));
  settings.setValue("particles", Enums::label(m_particles));
  settings.setValue("likelihoods", Enums::label(m_likelihoods));
  settings.setValue("particleFilter", Enums::label(m_particleFilter));
  settings.setValue("mcParticleFilter", Enums::label(m_mcParticleFilter));
  settings.setValue("cuts", m_cutFilter.toString());
  settings.endGroup();
}

void AnalysisProcessorSetting::load(const QString& file)
{
  clear();
  QSettings settings(file, QSettings::IniFormat);
  settings.beginGroup("AnalysisProcessorSetting");
  m_trackType = Enums::trackType(settings.value("trackType").toString());
  m_reconstructionMethods = Enums::reconstructionMethods(settings.value("reconstructionMethods").toString());
  m_reconstructionMethod = Enums::reconstructionMethod(settings.value("reconstructionMethod").toString());
  m_corrections = Enums::corrections(settings.value("corrections").toString());
  m_particles = Enums::particles(settings.value("particles").toString());
  m_likelihoods = Enums::likelihoodVariables(settings.value("likelihoods").toString());
  m_particleFilter = Enums::particles(settings.value("particleFilter").toString());
  m_mcParticleFilter = Enums::particles(settings.value("mcParticleFilter").toString());
  m_cutFilter = CutFilter(settings.value("cuts").toString());
  settings.endGroup();
}

QDebug operator<<(QDebug debug, const AnalysisProcessorSetting& setting)
{
  QString string;
  string+= "\nAnalysisProcessorSetting: trackType = " + Enums::label(setting.trackType())
    + ", reconstructionMethods = " + Enums::label(setting.reconstructionMethods())
    + ", reconstructionMethod = " + Enums::label(setting.reconstructionMethod())
    + ", corrections = " + Enums::label(setting.corrections())
    + ", particles = " + Enums::label(setting.particles())
    + ", likelihoods = " + Enums::label(setting.likelihoods())
    + ", particleFilter = " + Enums::label(setting.particleFilter())
    + ", mcParticleFilter = " + Enums::label(setting.mcParticleFilter())
    + ", cuts = " + setting.cutFilter().toString();
  return debug << qPrintable(string);
}

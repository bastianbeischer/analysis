#include "TestbeamRigidityResolutionPlot.hh"

#include "SimpleEvent.hh"
#include "ParticleProperties.hh"
#include "SettingsManager.hh"
#include "Settings.hh"

TestbeamRigidityResolutionPlot::TestbeamRigidityResolutionPlot(int pdgID) :
  RigidityResolutionPlot(AnalysisPlot::Testbeam, pdgID)
{
}

TestbeamRigidityResolutionPlot::~TestbeamRigidityResolutionPlot()
{
}

void TestbeamRigidityResolutionPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  //get settings if present
  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
  if (!settings || settings->situation() != Settings::Testbeam11)
    return;

  //only electrons for testbeam atm (tagged via cherenkov)
  if (m_particle->charge() != settings->polarity())
    return;

  double c1Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV1);
  double c2Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV2);

  if (m_particle->type() == Particle::Electron || m_particle->type() == Particle::Positron) {
    if (c1Signal < 200 || c2Signal < 200) // throw away all particles which DO NOT produce cherenkov light
      return;
  }

  if (m_particle->type() == Particle::Proton || m_particle->type() == Particle::PiPlus || m_particle->type() == Particle::PiMinus) {
    if (c1Signal > 200 || c2Signal > 200)  // throw away all particles which DO produce cherenkov light
      return;
  }

  RigidityResolutionPlot::processEvent(hits, particle, event);
}

double TestbeamRigidityResolutionPlot::referenceRigidity(SimpleEvent* event) const
{
  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
  double genMom = settings->momentum() * settings->polarity();
  return genMom / 1.; // electrons currently, but charge = 1 for all testbeam cases.
}

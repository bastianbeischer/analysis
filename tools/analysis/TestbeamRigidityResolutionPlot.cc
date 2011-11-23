#include "TestbeamRigidityResolutionPlot.hh"

#include "SimpleEvent.hh"
#include "ParticleProperties.hh"
#include "SettingsManager.hh"
#include "Settings.hh"

TestbeamRigidityResolutionPlot::TestbeamRigidityResolutionPlot(const Enums::Particle& type) :
  RigidityResolutionPlot(Enums::Testbeam, type)
{
}

TestbeamRigidityResolutionPlot::~TestbeamRigidityResolutionPlot()
{
}

void TestbeamRigidityResolutionPlot::processEvent(const QVector<Hit*>& hits, const Particle* const particle, const SimpleEvent* const event)
{
  //get settings if present
  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
  if (!settings || settings->situation() != Enums::Testbeam11)
    return;

  //only electrons for testbeam atm (tagged via cherenkov)
  if (m_particle->charge() != settings->polarity())
    return;

  double c1Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV1);
  double c2Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV2);

  double threshold = 200;
  if ( (c1Signal < threshold && c2Signal > threshold) || (c1Signal > threshold && c2Signal < threshold) ) // throw away all events where cherenkov info is inconsistent
    return;

  bool aboveThreshold = settings->isAboveThreshold(m_particle->mass());
  if (aboveThreshold && c1Signal < threshold && c2Signal < threshold) // throw away all particles which DO NOT produce cherenkov light
    return;
  else if (!aboveThreshold && c1Signal > threshold && c2Signal > threshold)  // throw away all particles which DO produce cherenkov light
    return;

  RigidityResolutionPlot::processEvent(hits, particle, event);
}

double TestbeamRigidityResolutionPlot::referenceRigidity(const SimpleEvent* const event) const
{
  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
  double genMom = settings->momentum() * settings->polarity();
  return genMom / 1.; // electrons currently, but charge = 1 for all testbeam cases.
}

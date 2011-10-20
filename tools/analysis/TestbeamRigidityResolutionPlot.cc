#include "TestbeamRigidityResolutionPlot.hh"

#include "SimpleEvent.hh"
#include "ParticleProperties.hh"
#include "Settings.hh"

TestbeamRigidityResolutionPlot::TestbeamRigidityResolutionPlot(const Enums::Particle& type) :
  RigidityResolutionPlot(Enums::Testbeam, type)
{
}

TestbeamRigidityResolutionPlot::~TestbeamRigidityResolutionPlot()
{
}

void TestbeamRigidityResolutionPlot::processEvent(const AnalyzedEvent* event)
{
  //get settings if present
  const Settings* settings = event->settings();
  if (!settings || settings->situation() != Settings::Testbeam11)
    return;

  //only electrons for testbeam atm (tagged via cherenkov)
  if (m_particle->charge() != settings->polarity())
    return;

  double c1Signal = event->simpleEvent()->sensorData(SensorTypes::BEAM_CHERENKOV1);
  double c2Signal = event->simpleEvent()->sensorData(SensorTypes::BEAM_CHERENKOV2);

  double threshold = 200;
  if ( (c1Signal < threshold && c2Signal > threshold) || (c1Signal > threshold && c2Signal < threshold) ) // throw away all events where cherenkov info is inconsistent
    return;

  bool aboveThreshold = settings->isAboveThreshold(m_particle->mass());
  if (aboveThreshold && c1Signal < threshold && c2Signal < threshold) // throw away all particles which DO NOT produce cherenkov light
    return;
  else if (!aboveThreshold && c1Signal > threshold && c2Signal > threshold)  // throw away all particles which DO produce cherenkov light
    return;

  RigidityResolutionPlot::processEvent(event);
}

double TestbeamRigidityResolutionPlot::referenceRigidity(const AnalyzedEvent* event) const
{
  double genMom = event->settings()->momentum() * event->settings()->polarity();
  return genMom / 1.; // electrons currently, but charge = 1 for all testbeam cases.
}

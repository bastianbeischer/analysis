#include "MCFilter.hh"

#include "SimpleEvent.hh"
#include "ParticleDB.hh"
#include "ParticleProperties.hh"

MCFilter::MCFilter(Enums::Particles types)
  : m_types(types)
{
}

bool MCFilter::passes(SimpleEvent* event) const
{
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return true;

  const ParticleProperties* particleProps = ParticleDB::instance()->lookupPdgId(event->MCInformation()->primary()->pdgID);
  return (particleProps->type() & m_types);
}

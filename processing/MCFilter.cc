#include "MCFilter.hh"

#include "SimpleEvent.hh"
#include "ParticleDB.hh"
#include "ParticleProperties.hh"

MCFilter::MCFilter(Types types) :
  m_types(types)
{
}

bool MCFilter::passes(const QVector<Hit*>&, Particle*, SimpleEvent* event) const
{
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return true;

  const ParticleProperties* particleProps = ParticleDB::instance()->lookupPdgId(event->MCInformation()->primary()->pdgID);
  return (particleProps->type() & m_types);
}

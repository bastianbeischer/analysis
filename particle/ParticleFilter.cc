#include "ParticleFilter.hh"

ParticleFilter::ParticleFilter(Types types) :
  m_types(types)
{
}

ParticleFilter::~ParticleFilter()
{
}

bool ParticleFilter::passes(Particle* particle) const
{
  return (particle->type() & m_types);
}

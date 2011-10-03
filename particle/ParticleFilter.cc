#include "ParticleFilter.hh"

#include "ParticleProperties.hh"

ParticleFilter::ParticleFilter(Enums::Particles types)
  : m_types(types)
{
}

ParticleFilter::~ParticleFilter()
{
}

bool ParticleFilter::passes(Particle* particle) const
{
  return (particle->properties()->type() & m_types);
}

#include "ParticleFilter.hh"

#include "Hypothesis.hh"

ParticleFilter::ParticleFilter(Enums::Particles types)
  : m_types(types)
{
}

ParticleFilter::~ParticleFilter()
{
}

bool ParticleFilter::passes(Particle* particle) const
{
  return (particle->hypothesis()->particle() & m_types);
}

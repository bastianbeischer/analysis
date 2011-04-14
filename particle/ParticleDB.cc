#include "ParticleDB.hh"

#include "ParticleProperties.hh"

#include <iostream>

ParticleDB* ParticleDB::m_instance = 0;

ParticleDB::ParticleDB()
{
  initializeDatabase();
}

ParticleDB::~ParticleDB()
{
  qDeleteAll(m_db);
}

ParticleDB* ParticleDB::instance()
{
  if (m_instance == 0) m_instance = new ParticleDB;
  return m_instance;
}

void ParticleDB::initializeDatabase()
{
  for (int type = Particle::START; type <= Particle::END; type = type<<1) {
    Particle::Type particleType = static_cast<Particle::Type>(type);
    const ParticleProperties* props = new ParticleProperties(particleType);
    int pdgId = props->pdgId();
    m_pdgIdMap[particleType] = pdgId;
    m_db[pdgId] = props;
  }
}

const ParticleProperties* ParticleDB::lookupPdgId(const int& pdgId)
{
  const ParticleProperties* props = m_db[pdgId];
  Q_ASSERT(props);
  return props;
}

const ParticleProperties* ParticleDB::lookupType(const Particle::Type& type)
{
  int pdgId = m_pdgIdMap[type];
  Q_ASSERT(pdgId);
  return lookupPdgId(pdgId);
}

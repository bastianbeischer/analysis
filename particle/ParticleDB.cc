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
  for (QMap<Enums::Particle, QString>::ConstIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it) {
    const ParticleProperties* props = new ParticleProperties(it.key());
    int pdgId = props->pdgId();
    m_pdgIdMap[it.key()] = pdgId;
    m_db[pdgId] = props;
  }
}

const ParticleProperties* ParticleDB::lookupPdgId(const int& pdgId)
{
  const ParticleProperties* props = m_db[pdgId];
  Q_ASSERT(props);
  return props;
}

const ParticleProperties* ParticleDB::lookupType(const Enums::Particle& type)
{
  int pdgId = m_pdgIdMap[type];
  return lookupPdgId(pdgId);
}

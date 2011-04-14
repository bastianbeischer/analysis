#ifndef ParticleDB_hh
#define ParticleDB_hh

#include "Particle.hh"

#include <QMap>

class ParticleProperties;

class ParticleDB
{
  
public:
  static ParticleDB* instance();
  ~ParticleDB();
  
  const ParticleProperties* lookupPdgId(const int&);
  const ParticleProperties* lookupType(const Particle::Type&);
  QList<const ParticleProperties*> allParticles() const {return m_db.values();}

private:
  ParticleDB();
  void initializeDatabase();
  
private:
  static ParticleDB* m_instance;
  QMap<Particle::Type, int> m_pdgIdMap;
  QMap<int, const ParticleProperties*> m_db;
  
};

#endif /* ParticleDB_hh */

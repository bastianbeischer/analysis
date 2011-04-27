#ifndef ParticleProperties_hh
#define ParticleProperties_hh

#include "Particle.hh"
#include <QString>

class ParticleProperties
{
  
public:
  ParticleProperties();
  ParticleProperties(const Particle::Type&);
  ~ParticleProperties();
  
  void setType(const Particle::Type&);

  Particle::Type type() const {return m_type;}
  int pdgId() const {return m_pdgId;}
  QString name() const {return m_name;}
  double mass() const {return m_mass;}
  int charge() const {return m_charge;}

private:
  Particle::Type m_type;
  int m_pdgId;
  QString m_name;
  double m_mass;
  int m_charge;

};

#endif /* ParticleProperties_hh */

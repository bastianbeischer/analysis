#ifndef ParticleProperties_hh
#define ParticleProperties_hh

#include "Enums.hh"

class ParticleProperties {
public:
  ParticleProperties();
  ParticleProperties(const Enums::Particle&);
  ~ParticleProperties();

  void setType(const Enums::Particle&);

  Enums::Particle type() const {return m_type;}
  int pdgId() const {return m_pdgId;}
  double mass() const {return m_mass;}
  int charge() const {return m_charge;}
  
  QString name() const;
  int color() const;

private:
  Enums::Particle m_type;
  int m_pdgId;
  double m_mass;
  int m_charge;
};

#endif /* ParticleProperties_hh */

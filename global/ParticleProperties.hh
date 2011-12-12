#ifndef ParticleProperties_hh
#define ParticleProperties_hh

#include "Enums.hh"

#include <QString>
#include <TColor.h>

class ParticleProperties
{
public:
  ParticleProperties();
  ParticleProperties(const Enums::Particle&);
  ~ParticleProperties();

  void setType(const Enums::Particle&);

  Enums::Particle type() const {return m_type;}
  int pdgId() const {return m_pdgId;}
  QString name() const {return m_name;}
  double mass() const {return m_mass;}
  int charge() const {return m_charge;}
  Color_t color() const {return m_color;}

private:
  Enums::Particle m_type;
  int m_pdgId;
  QString m_name;
  double m_mass;
  int m_charge;
  Color_t m_color;

};

#endif /* ParticleProperties_hh */

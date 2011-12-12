#ifndef Settings_hh
#define Settings_hh

#include "Enums.hh"

#include <QString>
#include <QVector>

class Settings
{
public:
  Settings();
  ~Settings();

  void setSituation(Enums::Situation situation) {m_situation = situation;}
  void setIdentifier(int identifier) {m_identifier = identifier;}
  void setName(const QString& name) {m_name = name;}
  void setFirstRun(int firstRun) {m_firstRun = firstRun;}
  void setLastRun(int lastRun) {m_lastRun = lastRun;}
  void setMomentum(double momentum) {m_momentum = momentum;}
  void setPolarity(int polarity) {m_polarity = polarity;}
  void setPressure(double pressure) {m_pressure = pressure;}
  void setMagnet(bool magnet) {m_magnet = magnet;}

  Enums::Situation situation() const {return m_situation;}
  int identifier() const {return m_identifier;}
  const QString& name() const {return m_name;}
  int firstRun() const {return m_firstRun;}
  int lastRun() const {return m_lastRun;}
  double momentum() const {return m_momentum;}
  int polarity() const {return m_polarity;}
  double pressure() const {return m_pressure;}
  bool magnet() const {return m_magnet;}

  Enums::Particles aboveThreshold(int* numberOfParticlesAboveThreshold = 0) const;
  bool isAboveThreshold(double) const;

private:
  Enums::Situation m_situation;
  int m_identifier;
  QString m_name;
  int m_firstRun;
  int m_lastRun;
  double m_momentum;
  int m_polarity;
  double m_pressure;
  bool m_magnet;
  
};

#endif /* Settings_hh */

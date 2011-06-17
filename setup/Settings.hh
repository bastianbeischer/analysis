#ifndef Settings_hh
#define Settings_hh

#include <QString>

class Settings
{
  
public:
  enum Situation {Unknown=-1, KirunaMuons, KirunaFlight, Testbeam11};

public:
  Settings();
  ~Settings();

  void setSituation(const Situation& situation) {m_situation = situation;}
  void setIdentifier(const int& identifier) {m_identifier = identifier;}
  void setName(const QString& name) {m_name = name;}
  void setFirstRun(const int& firstRun) {m_firstRun = firstRun;}
  void setLastRun(const int& lastRun) {m_lastRun = lastRun;}
  void setMomentum(const double& momentum) {m_momentum = momentum;}
  void setPolarity(const int& polarity) {m_polarity = polarity;}
  void setPressure(const double& pressure) {m_pressure = pressure;}
  void setMagnet(const bool& magnet) {m_magnet = magnet;}

  Situation situation() const {return m_situation;}
  int identifier() const {return m_identifier;}
  const QString& name() const {return m_name;}
  int firstRun() const {return m_firstRun;}
  int lastRun() const {return m_lastRun;}
  double momentum() const {return m_momentum;}
  int polarity() const {return m_polarity;}
  double pressure() const {return m_pressure;}
  bool magnet() const {return m_magnet;}

  bool isAboveThreshold(const double&) const;

private:
  Situation m_situation;
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

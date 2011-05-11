#ifndef Settings_hh
#define Settings_hh

#include <QString>
#include <QDebug>

class Settings
{
  
public:
  Settings();
  ~Settings();

  void setIdentifier(const int& identifier) {m_identifier = identifier;}
  void setName(const QString& name) {m_name = name;}
  void setFirstRun(const int& firstRun) {m_firstRun = firstRun;}
  void setLastRun(const int& lastRun) {m_lastRun = lastRun;}
  void setMomentum(const double& momentum) {m_momentum = momentum;}
  void setMagnet(const bool& magnet) {m_magnet = magnet;}

  int identifier() const {return m_identifier;}
  const QString& name() const {return m_name;}
  int firstRun() const {return m_firstRun;}
  int lastRun() const {return m_lastRun;}
  double momentum() const {return m_momentum;}
  bool magnet() const {return m_magnet;}
  
private:
  int m_identifier;
  QString m_name;
  int m_firstRun;
  int m_lastRun;
  double m_momentum;
  bool m_magnet;
  
};

#endif /* Settings_hh */

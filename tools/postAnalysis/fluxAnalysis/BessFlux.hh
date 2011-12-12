#ifndef BessFlux_hh
#define BessFlux_hh

#include "Particle.hh"

#include <QMap>
#include <QVector>
#include <QString>

class TH1D;

class BessFlux {
public:
  enum Type {MuPlus, MuMinus};
  static const BessFlux* instance();
  ~BessFlux();
  QVector<Type> types() const;
  TH1D* spectrum(Type) const;
  TH1D* spectrum(const QList<Type>&) const;
  Type type(const QString&) const;
  QString typeName(Type) const;
private:
  BessFlux();
  static BessFlux* s_instance;
  QMap<Type, TH1D*> m_fluxes;
  QMap<Type, QString> m_typeNames;
};

#endif

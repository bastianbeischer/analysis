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
  static BessFlux* instance();
  ~BessFlux();
  QVector<Type> types() const;
  TH1D* spectrum(Type) const;
  TH1D* spectrum(const QList<Type>&) const;

  static Type type(const QString&);
  static QString typeName(Type);

private:
  BessFlux();
  static BessFlux* s_instance;
  QMap<Type, TH1D*> m_fluxes;
  static QMap<Type, QString> s_typeNames;
  static void construct();
};

#endif

#ifndef Enums_hh
#define Enums_hh

#include <QMap>
#include <QPair>

namespace Enums {
  enum ChargeSign {Positive = 1<<0, Negative = 1<<1};
  const QString label(ChargeSign);
  ChargeSign chargeSign(const QString&);
  Q_DECLARE_FLAGS(ChargeSigns, ChargeSign);
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::ChargeSigns);

#endif

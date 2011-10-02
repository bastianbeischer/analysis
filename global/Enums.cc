#include "Enums.hh"

template <class Key, class T> class InitializableMap : public QMap<Key, T> {
public:
  inline InitializableMap<Key, T>& operator<< (const QPair<Key, T>& pair) {
    insert(pair.first, pair.second);
    return *this;
  }
};

namespace Enums {

  const QMap<ChargeSign, QString> s_chargeSignMap = InitializableMap<ChargeSign, QString>()
    << QPair<ChargeSign, QString>(Positive, "positive")
    << QPair<ChargeSign, QString>(Negative, "negative");
  const QString label(ChargeSign key)
  {
    return s_chargeSignMap.value(key);
  }
  ChargeSign chargeSign(const QString& value)
  {
    return s_chargeSignMap.key(value);
  }

}

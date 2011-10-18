#ifndef Caption_hh
#define Caption_hh

#include <QWidget>

class Caption : public QWidget {
public:
  Caption(const QString&, QWidget* = 0);
  ~Caption();
};

#endif

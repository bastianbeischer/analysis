#ifndef BessFluxWidget_hh
#define BessFluxWidget_hh

#include "BessFluxSelector.hh"

class BessFluxWidget : public BessFluxSelector {
Q_OBJECT
public:
  BessFluxWidget(QWidget* = 0);
  ~BessFluxWidget();
private slots:
  void update();
};

#endif

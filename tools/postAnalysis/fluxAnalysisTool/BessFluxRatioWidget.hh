#ifndef BessFluxRatioWidget_hh
#define BessFluxRatioWidget_hh

#include "BessFluxSelector.hh"

class BessFluxRatioWidget : public BessFluxSelector {
Q_OBJECT
public:
  BessFluxRatioWidget(QWidget* = 0);
  ~BessFluxRatioWidget();
private slots:
  void update();
};

#endif

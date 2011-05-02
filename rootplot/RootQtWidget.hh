#ifndef RootQtWidget_hh
#define RootQtWidget_hh

#include <TQtWidget.h>

class QMouseEvent;

class RootQtWidget :
  public TQtWidget
{
  
Q_OBJECT

public:
  RootQtWidget(QWidget* = 0);
  ~RootQtWidget();

public:
  void updateCanvas();

signals:
  void unzoomButtonPressed();
  void positionChanged(double, double);

public slots:
  void setGrid(int);
  void setLogX(int);
  void setLogY(int);
  void setLogZ(int);

private:
  void mousePressEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent*);

};

#endif /* RootQtWidget_hh */

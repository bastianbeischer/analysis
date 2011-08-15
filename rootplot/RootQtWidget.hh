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
  void setAspectRatio(double);
public slots:
  void setGrid(int);
  void setLogX(int);
  void setLogY(int);
  void setLogZ(int);
signals:
  void unzoomButtonPressed();
  void positionChanged(double, double);
private:
  void resizeEvent(QResizeEvent*);
  void mousePressEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent*);

  bool m_inhibitResizeEvent;
  double m_aspectRatio;
};

#endif /* RootQtWidget_hh */

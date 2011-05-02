#ifndef ProjectionControlWidget_hh
#define ProjectionControlWidget_hh

#include <QWidget>

class ProjectionControlWidget :
  public QWidget
{
  
  Q_OBJECT

public:
  ProjectionControlWidget(QWidget* = 0);
  ~ProjectionControlWidget();
  
signals:
  void showProjectionChanged(int);
  
};

#endif /* ProjectionControlWidget_hh */

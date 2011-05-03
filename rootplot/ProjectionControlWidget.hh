#ifndef ProjectionControlWidget_hh
#define ProjectionControlWidget_hh

#include <QWidget>

class H2DProjectionPlot;
class QComboBox;
class QSpinBox;

class ProjectionControlWidget :
  public QWidget
{
  
  Q_OBJECT

public:
  ProjectionControlWidget(H2DProjectionPlot*, QWidget* = 0);
  ~ProjectionControlWidget();
  
private slots:
  void switchState(bool);
  void changeProjectionType(int);

private:
  H2DProjectionPlot* m_projectionPlot;

  QComboBox* m_comboBox;
  QSpinBox* m_spinBox;

};

#endif /* ProjectionControlWidget_hh */

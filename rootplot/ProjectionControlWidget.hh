#ifndef ProjectionControlWidget_hh
#define ProjectionControlWidget_hh

#include <QWidget>

#include "H2DProjectionPlot.hh"

class QComboBox;
class QSpinBox;

class ProjectionControlWidget :
  public QWidget
{
  Q_OBJECT
public:
  ProjectionControlWidget(H2DProjectionPlot*, QWidget* = 0);
  ~ProjectionControlWidget();
  QSpinBox* spinBox();
  void setProjectionType(H2DProjectionPlot::ProjectionType);
private slots:
  void changeProjectionType(int);
private:
  H2DProjectionPlot* m_projectionPlot;
  QComboBox* m_comboBox;
  QSpinBox* m_spinBox;
};

#endif /* ProjectionControlWidget_hh */

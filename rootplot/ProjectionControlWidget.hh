#ifndef ProjectionControlWidget_hh
#define ProjectionControlWidget_hh

#include <QWidget>

#include "H2DProjectionPlot.hh"

class QComboBox;
class QSpinBox;
class QCheckBox;

class ProjectionControlWidget :
  public QWidget
{
  Q_OBJECT
public:
  ProjectionControlWidget(H2DProjectionPlot*, QWidget* = 0);
  ~ProjectionControlWidget();
  QSpinBox* spinBox();
  QComboBox* comboBox();
  void setProjectionType(H2DProjectionPlot::ProjectionType);

private:
  void setElementStatus(bool);

private slots:
  void changeProjectionType(int);

private:
  H2DProjectionPlot* m_projectionPlot;
  QComboBox* m_comboBox;
  QSpinBox* m_spinBox;
  QCheckBox* m_checkBoxLogX;
  QCheckBox* m_checkBoxLogY;
};

#endif /* ProjectionControlWidget_hh */

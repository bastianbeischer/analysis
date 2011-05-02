#ifndef H2DProjectionPlot_hh
#define H2DProjectionPlot_hh

#include <QObject>

#include "H2DPlot.hh"

class ProjectionControlWidget;
class RootQtWidget;

class H2DProjectionPlot :
  public QObject,
  public H2DPlot
{
  
  Q_OBJECT

public:
  H2DProjectionPlot();
  ~H2DProjectionPlot();
  
public:
  void positionChanged(double, double);

protected:
  RootQtWidget* projectionWidget() const {return m_projectionWidget;}

protected slots:
  void toggleProjectionWidget(int);

private:
  ProjectionControlWidget* m_controlWidget;
  RootQtWidget* m_projectionWidget;
  
};

#endif /* H2DProjectionPlot_hh */

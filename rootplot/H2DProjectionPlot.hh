#ifndef H2DProjectionPlot_hh
#define H2DProjectionPlot_hh

#include <QObject>

#include "H2DPlot.hh"

class ProjectionControlWidget;
class RootQtWidget;
class TH1D;

class H2DProjectionPlot :
  public QObject,
  public H2DPlot
{
  
  Q_OBJECT

public:
  enum ProjectionType {x, y};

public:
  H2DProjectionPlot();
  ~H2DProjectionPlot();
  
public:
  void positionChanged(double, double);

protected:
  RootQtWidget* projectionWidget() const {return m_projectionWidget;}

public slots:
  void setProjectionWidgetState(bool);
  void setProjectionType(ProjectionType type);
  void setNumberOfBins(int);

private:
  TH1D* m_projectionHistX;
  TH1D* m_projectionHistY;

  ProjectionControlWidget* m_controlWidget;
  RootQtWidget* m_projectionWidget;
  
  ProjectionType m_type;
  int m_numberOfBins;

};

#endif /* H2DProjectionPlot_hh */

#ifndef H2DProjectionPlot_hh
#define H2DProjectionPlot_hh

#include <QObject>

#include "H2DPlot.hh"

class ProjectionControlWidget;
class RootQtWidget;
class TH1D;
class TLine;

class H2DProjectionPlot : public QObject, public H2DPlot {
Q_OBJECT
public:
  enum ProjectionType {NoProjection, ProjectionOnX, ProjectionOnY};

public:
  H2DProjectionPlot();
  ~H2DProjectionPlot();

public:
  void positionChanged(double, double);

protected:
  ProjectionControlWidget* controlWidget() const {return m_controlWidget;}
  RootQtWidget* projectionWidget() const {return m_projectionWidget;}

public slots:
  void setProjectionType(ProjectionType type);
  void setLogX(int);
  void setLogY(int);
  void setLogZ(int);

private:
  TH1D* m_projectionHistX;
  TH1D* m_projectionHistY;
  ProjectionControlWidget* m_controlWidget;
  RootQtWidget* m_projectionWidget;
  ProjectionType m_type;
  TLine* m_line;
};

#endif /* H2DProjectionPlot_hh */

#ifndef H2DProjectionPlot_hh
#define H2DProjectionPlot_hh

#include <QObject>

#include "H2DPlot.hh"

class ProjectionControlWidget;
class RootQtWidget;
class TH1D;
class TLine;
class TF1;
class TLegend;
class TLatex;

class H2DProjectionPlot : public QObject, public H2DPlot {
Q_OBJECT
public:
  enum ProjectionType {NoProjection, ProjectionOnX, ProjectionOnY};

public:
  H2DProjectionPlot();
  ~H2DProjectionPlot();

public:
  virtual void positionChanged(double, double);
  void addProjectionLatex(TLatex*);
  TLatex* projectionLatex(int i = 0);
  void addProjectionLegend(TLegend*);
  TLegend* projectionLegend(int i = 0);
  void addProjectionFunction(TF1*);
  TF1* projectionFunction(int i = 0);
  int projectionBin() const {return m_bin;}

protected:
  ProjectionControlWidget* controlWidget() const {return m_controlWidget;}
  RootQtWidget* projectionWidget() const {return m_projectionWidget;}
  TH1D* projection();
  virtual void updateProjection();
  ProjectionType projectionType() const {return m_type;}
  ProjectionControlWidget* projectionControlWidget() {return m_controlWidget;}

public slots:
  void setProjectionType(ProjectionType type);
  void setLogX(int);
  void setLogY(int);
  void setLogZ(int);

private:
  TH1D* m_projection;
  ProjectionControlWidget* m_controlWidget;
  RootQtWidget* m_projectionWidget;
  ProjectionType m_type;
  TLine* m_line;
  int m_bin;
  QVector<TLatex*> m_projectionLatex;
  QVector<TLegend*> m_projectionLegend;
  QVector<TF1*> m_projectionFunction;
};

#endif /* H2DProjectionPlot_hh */

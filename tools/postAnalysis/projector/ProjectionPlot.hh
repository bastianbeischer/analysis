#ifndef ProjectionPlot_hh
#define ProjectionPlot_hh

#include <QObject>

#include "PostAnalysisPlot.hh"
#include "H2DPlot.hh"

class TH1D;
class TPolyLine;
class TLatex;
class TLine;
class TCanvas;
class TVirtualPad;
class DoubleEdit;

class ProjectionPlot : public QObject, public PostAnalysisPlot, public H2DPlot {
Q_OBJECT
public:
  ProjectionPlot(TH2D*);
  virtual ~ProjectionPlot();
  virtual void draw(TCanvas*);
public slots:
  virtual void update();
private:
  TH2D* m_histogram;
  TH1D* m_xProjection;
  TH1D* m_yProjection;
  TPolyLine* m_area;
  QVector<TLine*> m_xLines;
  QVector<TLine*> m_yLines;
  QVector<TLine*> m_statisticsLines;
  QVector<TLatex*> m_xLabels;
  QVector<TLatex*> m_yLabels;
  QVector<TLatex*> m_statisticsLabels;
  TPolyLine* m_statisticsArea;
  QVector<TVirtualPad*> m_pads;
  DoubleEdit* m_xMinCutEdit;
  DoubleEdit* m_xMaxCutEdit;
  DoubleEdit* m_yMinCutEdit;
  DoubleEdit* m_yMaxCutEdit;
};

#endif

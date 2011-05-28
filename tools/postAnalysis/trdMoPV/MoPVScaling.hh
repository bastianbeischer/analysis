#ifndef MoPVScaling_hh
#define MoPVScaling_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QList>
#include <QObject>
class QPushButton;

class PostAnalysisCanvas;
class TMultiGraph;
class TGraphErrors;
class TH2D;
class TF1;
class TSpline3;

class MoPVScaling : public QObject, public PostAnalysisPlot, public GraphPlot
{
  Q_OBJECT
public:
  MoPVScaling(PostAnalysisCanvas*);
  MoPVScaling(QList<PostAnalysisCanvas*>);
  virtual ~MoPVScaling();
  virtual void draw(TCanvas*);

private:
  TGraphErrors* generateMoPVGraph(TH2D* histogram);

private:
  int m_colorCounter;
  QList<TGraphErrors*> m_mopvGraphs;

protected slots:
  virtual void saveDependency();

protected:
  TF1* m_dependencyFit;
  TSpline3* m_spline;
};

#endif

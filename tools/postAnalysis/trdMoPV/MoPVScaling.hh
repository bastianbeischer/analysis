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

class MoPVScaling : public QObject, public PostAnalysisPlot, public GraphPlot
{
  Q_OBJECT
public:
  MoPVScaling(PostAnalysisCanvas*);
  MoPVScaling(QList<PostAnalysisCanvas*>);
  virtual ~MoPVScaling();

private:
  int m_colorCounter;

  QList<TGraphErrors*> m_mopvGraphs;
  TGraphErrors* generateMoPVGraph(TH2D* histogram);

protected:
  TF1* m_dependencyFit;
protected slots:
  virtual void saveDependency();
};

#endif

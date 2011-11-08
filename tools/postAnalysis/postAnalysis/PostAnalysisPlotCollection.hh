#ifndef PostAnalysisPlotCollection_hh
#define PostAnalysisPlotCollection_hh

#include "PostAnalysisPlot.hh"

#include <QObject>

class TCanvas;
class PostAnalysisCanvas;

class PostAnalysisPlotCollection : public QObject, public PostAnalysisPlot
{
  Q_OBJECT
public:
  PostAnalysisPlotCollection();
  virtual ~PostAnalysisPlotCollection();
  
  void addPlot(PostAnalysisPlot*, PostAnalysisCanvas*);
  virtual void draw(TCanvas*);
  virtual bool isPostAnalysisPlotCollection();

  void unzoom();
  void saveForPostAnalysis(TCanvas*);
  void positionChanged(double, double);

protected slots:
  void selectPlot(int);

private:
  int m_selectedPlot;
  QVector<PostAnalysisPlot*> m_plots;
  QVector<PostAnalysisCanvas*> m_canvas;
};

#endif /* PostAnalysisPlotCollection_hh */

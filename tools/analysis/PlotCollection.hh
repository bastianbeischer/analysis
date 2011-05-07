#ifndef PlotCollection_hh
#define PlotCollection_hh

#include "AnalysisPlot.hh"
#include <QObject>

class PlotCollection :
  public QObject,
  public AnalysisPlot
{
  Q_OBJECT

public:
  PlotCollection(AnalysisPlot::Topic);
  ~PlotCollection();
  
  void addPlot(AnalysisPlot* plot) {m_plots.append(plot);}
  void processEvent(const QVector<Hit*>&, Particle*, SimpleEvent*);
  void finalize();
  void update();
  void unzoom();
  void draw(TCanvas*);
  void saveForPostAnalysis(TCanvas*);

protected slots:
  void selectPlot(int);

private:
  int m_selectedPlot;
  QVector<AnalysisPlot*> m_plots;
  
};

#endif /* PlotCollection_hh */

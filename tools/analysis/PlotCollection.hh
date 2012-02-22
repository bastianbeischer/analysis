#ifndef PlotCollection_hh
#define PlotCollection_hh

#include "AnalysisPlot.hh"

#include <QObject>

class TCanvas;

class PlotCollection : public QObject, public AnalysisPlot
{
  Q_OBJECT
public:
  PlotCollection(Enums::AnalysisTopic);
  virtual ~PlotCollection();

  int numberOfPlots() const {return m_plots.count();}
  AnalysisPlot* plot(int i) {Q_ASSERT(0 <= i && i < m_plots.count()); return m_plots[i];}
  void addPlot(AnalysisPlot* plot) {m_plots.append(plot);}
  virtual void processEvent(const AnalyzedEvent*);
  virtual void finalize();
  virtual void update();
  virtual void draw(TCanvas*);
  virtual bool isPlotCollection();

  void unzoom();
  void saveForPostAnalysis(TCanvas*);
  void positionChanged(double, double);

protected slots:
  void selectPlot(int);

private:
  int m_selectedPlot;
  QVector<AnalysisPlot*> m_plots;

};

#endif /* PlotCollection_hh */

#ifndef Plotter_hh
#define Plotter_hh

#include "AnalysisPlot.hh"

#include <TQtWidget.h>

#include <QVector>
#include <QTimer>

class AnalysisPlot;

class Plotter : public TQtWidget
{
Q_OBJECT
public:
  Plotter(QWidget* parent = 0);
  virtual ~Plotter();
  QVector<AnalysisPlot*> plots();
  unsigned int numberOfPlots();
  void addPlot(AnalysisPlot*);
  void clearPlots();
  QVector<unsigned int> plotIndices(AnalysisPlot::Topic topic);
  const QString& plotTitle(unsigned int);
  AnalysisPlot::Topic plotTopic(unsigned int);
  void selectPlot(int, bool = false);
  RootPlot::DrawOption drawOption();
  RootPlot::Type selectedPlotType();
  void unzoom();
  void setGrid(bool);
  void setLogX(bool);
  void setLogY(bool);
  void setLogZ(bool);
  void saveCanvas(const QString& fileName);
  void saveForPostAnalysis(const QString&);
public slots:
  void update();
  void finalizeAnalysis();
  void toggleUpdateTimer();
signals:
  void titleChanged(const QString&);
  void positionChanged(double, double);
protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent* event);
  void updateCanvas();
private:
  QTimer m_updateTimer;
  QVector<AnalysisPlot*> m_plots;
  int m_selectedPlot;
};

#endif /* Plotter_hh */

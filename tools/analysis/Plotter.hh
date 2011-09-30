#ifndef Plotter_hh
#define Plotter_hh

#include "AnalysisPlot.hh"

#include <QWidget>
#include <QVector>
#include <QTimer>

class QVBoxLayout;
class RootQtWidget;
class AnalysisPlot;
class QResizeEvent;

class Plotter : public QWidget
{
Q_OBJECT
public:
  Plotter(QWidget* parent = 0);
  virtual ~Plotter();
  QVector<AnalysisPlot*> plots();
  unsigned int numberOfPlots();
  void addPlot(AnalysisPlot*);
  void clearPlots();
  QVector<unsigned int> plotIndices(AnalysisTopic topic);
  const QString& plotTitle(unsigned int);
  AnalysisTopic plotTopic(unsigned int);
  void selectPlot(int);
  RootPlot::DrawOption drawOption();
  void setDrawOption(RootPlot::DrawOption);
  void setAspectRatio(double);
  RootPlot::Type selectedPlotType();
  void saveCanvas(const QString& fileName);
  void saveForPostAnalysis(const QString&);
  static RootQtWidget* rootWidget();
public slots:
  void update();
  void unzoom();
  void canvasPositionChanged(double, double);
  void finalizeAnalysis();
  void toggleUpdateTimer();
signals:
  void titleChanged(const QString&);
  void positionChanged(double, double);
private:
  QVBoxLayout* m_layout;
  static RootQtWidget* s_rootWidget;
  QTimer m_updateTimer;
  QVector<AnalysisPlot*> m_plots;
  int m_selectedPlot;
};

#endif /* Plotter_hh */

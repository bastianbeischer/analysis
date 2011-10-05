#ifndef Plotter_hh
#define Plotter_hh

#include "AnalysisPlot.hh"
#include "Enums.hh"

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
  void clear();
  AnalysisPlot* plot();
  void setPlot(AnalysisPlot*);
  void setAspectRatio(double);
  void saveCanvas(const QString& fileName);
  static RootQtWidget* rootWidget();
signals:
  void positionChanged(double, double);
public slots:
  void update();
  void unzoom();
  void canvasPositionChanged(double, double);
  void toggleUpdateTimer();
private:
  static RootQtWidget* s_rootWidget;
  QVBoxLayout* m_layout;
  AnalysisPlot* m_plot;
  QTimer m_updateTimer;
};

#endif /* Plotter_hh */

#ifndef Plotter_hh
#define Plotter_hh

#include "AnalysisPlot.hh"
#include "Track.hh"
#include "Corrections.hh"

#include <TQtWidget.h>

#include <QVector>
#include <QTime>
#include <QTimer>

class QLabel;
class QProgressBar;
class AnalysisPlot;
class DataChain;

class Plotter : public TQtWidget
{
Q_OBJECT
public:
  Plotter(QWidget* parent = 0);
  virtual ~Plotter();
  unsigned int numberOfPlots();
  void addPlot(AnalysisPlot*);
  void clearPlots();
  QVector<unsigned int> plotIndices(AnalysisPlot::Topic topic);
  const QString& plotTitle(unsigned int);
  AnalysisPlot::Topic plotTopic(unsigned int);
  void selectPlot(int, bool = false);
  void setTitleLabel(QLabel*);
  void setPositionLabel(QLabel*);
  void setTimeLabel(QLabel*);
  void unzoom();
  void setGrid(bool);
  void setLogX(bool);
  void setLogY(bool);
  void setLogZ(bool);
  void setDataChainProgressBar(QProgressBar*);
  void setEventQueueProgressBar(QProgressBar*);
  void saveCanvas(const QString& fileName);
  void saveForPostAnalysis(const QString&);
  void setFileList(const QString& fileName);
  void addFileList(const QString& fileName);
  void addRootFile(const QString& file);
  void setFirstEvent(int);
  void setLastEvent(int);
public slots:
  void startAnalysis(Track::Type, Corrections::Flags, int numberOfThreads);
  void abortAnalysis();
protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent* event);
  void updateCanvas();
  void finalizeAnalysis();
protected slots:
  void update();
signals:
  void numberOfEventsChanged(int);
  void analysisStarted();
  void analysisCompleted();
private:
  QLabel* m_titleLabel;
  QLabel* m_positionLabel;
  QLabel* m_timeLabel;
  QTime m_time;
  QTimer m_updateTimer;
  QProgressBar* m_dataChainProgressBar;
  QProgressBar* m_eventQueueProgressBar;
  QVector<AnalysisPlot*> m_plots;
  DataChain* m_chain;
  unsigned int m_firstEvent;
  unsigned int m_lastEvent;
  bool m_eventLoopOff;
  int m_selectedPlot;
};

#endif /* Plotter_hh */

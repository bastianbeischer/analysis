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

class Plotter : public TQtWidget {
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
  void selectPlot(int);
  void setTitleLabel(QLabel*);
  void setPositionLabel(QLabel*);
  void setTimeLabel(QLabel*);
  void setGrid(bool);
  void setFirstEvent(int);
  void setLastEvent(int);
  void setDataChainProgressBar(QProgressBar*);
  void setEventQueueProgressBar(QProgressBar*);
  void saveCanvas(const QString& fileName);
  void saveForPostAnalysis(const QString&);
  void setFileList(const QString& fileName);
  void addFileList(const QString& fileName);
  void addRootFile(const QString& file);
public slots:
  void startAnalysis(Track::Type, Corrections::Flags, int numberOfThreads);
  void abortAnalysis();
  void finalizeAnalysis();
  void update();
protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent *event);
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
  int m_firstEvent;
  int m_lastEvent;
  bool m_eventLoopOff;
  int m_selectedPlot;
};

#endif /* Plotter_hh */

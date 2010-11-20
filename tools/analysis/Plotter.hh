#ifndef Plotter_hh
#define Plotter_hh

#include "AnalysisPlot.hh"
#include "Track.hh"

#include <TQtWidget.h>

#include <QVector>

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
  void setDataChainProgressBar(QProgressBar*);
  void setEventQueueProgressBar(QProgressBar*);
  void saveCanvas(const QString& fileName);
  void addFileList(const QString& fileName);
  void setFileList(const QString& fileName);
public slots:
  void startAnalysis(Track::Type, int numberOfThreads);
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
  QProgressBar* m_dataChainProgressBar;
  QProgressBar* m_eventQueueProgressBar;
  QVector<AnalysisPlot*> m_plots;
  DataChain* m_chain;
  bool m_eventLoopOff;
  int m_selectedPlot;
};

#endif /* Plotter_hh */

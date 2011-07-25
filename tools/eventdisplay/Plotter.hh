#ifndef Plotter_hh
#define Plotter_hh

#include <TQtWidget.h>

#include "Track.hh"

#include <QString>

class AnalysisProcessor;
class TCanvas;
class DataChain;
class TrackFinding;
class HitsPlot;
class QLabel;
class QPlainTextEdit;

class Plotter : public TQtWidget {
Q_OBJECT
public:
  Plotter(QWidget* parent = 0);
  virtual ~Plotter();
public:
  unsigned int numberOfEvents();
  void setFileList(const QString& listName);
  void addFileList(const QString& listName);
  void addRootFile(const QString&);
  void drawEvent(unsigned int i, Track::Type type, bool allClusters, QPlainTextEdit& infoTextBox, TQtWidget& trackFindingWidget);
  void saveCanvas(const QString& fileName);
  void setPositionLabel(QLabel* label);
public slots:
  void unzoom();
  void updateTrackFindingParameters(int slopeBins, int offsetBins, double trackerPull, double trdPull);
protected:
  void mouseMoveEvent(QMouseEvent* event);
private:
  TCanvas* m_canvas;
  DataChain* m_chain;
  TrackFinding* m_trackFinding;
  Track* m_track;
  AnalysisProcessor* m_processor;
  HitsPlot* m_hitsPlot;
  QLabel* m_positionLabel;
};

#endif /* Plotter_hh */

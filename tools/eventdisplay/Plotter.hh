#ifndef Plotter_hh
#define Plotter_hh

#include <TQtWidget.h>

#include <QString>
#include <QVector3D>

class TCanvas;
class DataChain;
class TrackFinding;
class Track;
class HitsPlot;
class QLabel;

class Plotter : public TQtWidget {
Q_OBJECT
public:
  Plotter(QWidget* parent = 0);
  virtual ~Plotter();
public:
  unsigned int numberOfEvents();
  void setFileList(const QString& listName);
  void addFileList(const QString& listName);
  void drawEvent(unsigned int i, bool drawTrack);
  void saveCanvas(const QString& fileName);
  void setPositionLabel(QLabel* label);
public slots:
  void unzoom();
protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent *event);
private:
  TCanvas* m_canvas;
  DataChain* m_chain;
  TrackFinding* m_trackFinding;
  Track* m_track;
  HitsPlot* m_hitsPlot;
  QLabel* m_positionLabel;
};

#endif /* Plotter_hh */

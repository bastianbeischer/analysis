#ifndef TRDOCCUPANCYPLOT_HH
#define TRDOCCUPANCYPLOT_HH

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include "TEllipse.h"

#include <QObject>
#include <QMap>
#include <QTime>
#include <QTimer>

class TRDOccupancyPlot : public QObject, public AnalysisPlot, public H2DPlot
{
public:
  enum TrdOccupancyType{numberOfHits, sumOfSignalHeights, sumOfSignalHeightsNormalizedToHits};

public:
  TRDOccupancyPlot(TrdOccupancyType, bool onlyOnTrack = false);
  ~TRDOccupancyPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();
  virtual void draw(TCanvas* c);
  virtual void update();

private:
  QMap< unsigned short, unsigned int > m_hits;
  QMap< unsigned short, double > m_signalHeightSum;
  QMap< unsigned short, TEllipse* > m_ellipses;

  bool m_ellipsesDrawn;

  void initializeEllipses();
  void updateEllipses();

  TrdOccupancyType m_occupancyType;
  bool m_onlyOnTrack;

};

#endif // TRDOCCUPANCYPLOT_HH

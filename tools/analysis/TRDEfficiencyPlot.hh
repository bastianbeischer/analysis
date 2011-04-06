#ifndef TRDEFFICIENCYPLOT_HH
#define TRDEFFICIENCYPLOT_HH

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include "TEllipse.h"

#include <QObject>
#include <QMap>
#include <QTime>
#include <QTimer>
#include <QVector2D>

class TRDEfficiencyPlot : public QObject, public AnalysisPlot, public H2DPlot
{

public:
  TRDEfficiencyPlot();
  ~TRDEfficiencyPlot();
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  virtual void finalize();
  virtual void draw(TCanvas* c);
  virtual void update();

private:
  QMap< unsigned short, unsigned long > m_hits;
  QMap< unsigned short, unsigned long > m_wasOnTrack;
  QMap< unsigned short, TEllipse* > m_ellipses;

  QMap< unsigned short, QVector2D > m_trdChanPositions;

  bool m_ellipsesDrawn;

  void initializeEllipses();
  void updateEllipses();

};

#endif // TRDEFFICIENCYPLOT_HH

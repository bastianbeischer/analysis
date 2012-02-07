#include "TOFStartTimePlot.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Constants.hh"
#include "ProjectionControlWidget.hh"
#include "AnalyzedEvent.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>
#include <QSpinBox>

TOFStartTimePlot::TOFStartTimePlot()
  : AnalysisPlot(Enums::MiscellaneousTOF)
  , H2DProjectionPlot()
{
  controlWidget()->spinBox()->setMaximum(64);
  QString title = QString("tof starttime");
  setTitle(title);
  const int nBinsTime = 2000;
  const double minTime = 0;
  const double maxTime= 200;
  TH2D* histogram = new TH2D(qPrintable(title), "", 64, -0.5, 63.5, nBinsTime, minTime, maxTime);
  for (int ch = 0; ch < 64; ++ch)
    histogram->GetXaxis()->SetBinLabel(1 + ch, qPrintable(QString("%1").arg(0x8000 | ch, 0, 16)));
  setAxisTitle("", "starttime / ns", "");
  addHistogram(histogram);
}

TOFStartTimePlot::~TOFStartTimePlot()
{}

void TOFStartTimePlot::processEvent(const AnalyzedEvent* event)
{
  const QVector<Hit*>& hits = QVector<Hit*>::fromStdVector(event->simpleEvent()->hits());
  const QVector<Hit*>::const_iterator endIt = hits.end();
  for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      const std::vector<Hit*>& subHits = cluster->hits();
      std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator it = subHits.begin(); it != subHitsEndIt; ++it) {
        Hit* tofHit = *it;
        TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
        histogram()->Fill(tofHit->detId() - 0x8000, tofSipmHit->startTime());
      }
    }
  }
}

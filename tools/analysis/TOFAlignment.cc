#include "TOFAlignment.hh"
#include "BrokenLine.hh"
#include "TrackInformation.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"

#include <TH1.h>
#include <TVector3.h>
#include <TLatex.h>

#include <QDebug>

TOFAlignment::TOFAlignment()
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H1DPlot()
{
  setTitle("tof alignment");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 1, 0, 1);
  addHistogram(histogram);
}

TOFAlignment::~TOFAlignment()
{}

void TOFAlignment::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  if (track->p() < 2)
    return;

  TrackInformation::Flags flags = track->information()->flags();

  QString output;
  int counter = 0;
  foreach(Hit* cluster, clusters) {
    if (cluster->type() == Hit::tof) {
      TOFCluster* tofCluster= static_cast<TOFCluster*>(cluster);
      for (unsigned int i = 0; i < tofCluster->hits().size(); ++i) {
        TOFSipmHit* hit = static_cast<TOFSipmHit*>(tofCluster->hits()[i]);
        double bending = track->x(cluster->position().z()) - cluster->position().x();
        double nonBending = track->y(cluster->position().z());
        if (qAbs(nonBending) > 100 || qAbs(bending) > 20)
          return;
        output+= QString("%1 %2  ").arg(hit->detId(), 0, 16).arg(hit->startTime(), 5, 'f', 2);
        ++counter;
      }
    }
  }
  output+= QString("%1 ").arg(track->x(Constants::upperTofPosition), 6, 'f', 2);
  output+= QString("%1 ").arg(track->y(Constants::upperTofPosition), 6, 'f', 2);
  output+= QString("%1 ").arg(track->x(Constants::lowerTofPosition), 6, 'f', 2);
  output+= QString("%1 ").arg(track->y(Constants::lowerTofPosition), 6, 'f', 2);
  output+= QString("%1").arg(track->trackLength(), 6, 'f', 2);
  if (counter == 16 || counter == 15)
    qDebug() << output;
}

#include "ClusterLengthPlot.hh"

#include "Track.hh"
#include "TrackInformation.hh"
#include <TH1D.h>

#include "Hit.hh"
#include "Cluster.hh"

ClusterLengthPlot::ClusterLengthPlot(AnalysisPlot::Topic topic, unsigned short id) :
  AnalysisPlot(topic),
  H1DPlot(),
  m_id(id)
{
  setTitle(QString("cluster length 0x%1").arg(m_id,0,16));
  TH1D* histogram = new TH1D(qPrintable(title()), "", 10, 0, 10);
  addHistogram(histogram);
}

ClusterLengthPlot::~ClusterLengthPlot()
{
}

void ClusterLengthPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*)
{
  if (!track)
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  foreach(Hit* hit, hits) {
    Cluster* cluster = static_cast<Cluster*>(hit);
    if (hit->detId() - hit->channel() == m_id)
      histogram()->Fill(cluster->hits().size());
  }
}

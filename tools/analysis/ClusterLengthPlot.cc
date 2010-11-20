#include "ClusterLengthPlot.hh"

#include "TrackSelection.hh"
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

void ClusterLengthPlot::processEvent(const QVector<Hit*>& hits, Track*, TrackSelection* selection, SimpleEvent*)
{
  if (!selection)
    return;

  TrackSelection::Flags flags = selection->flags();
  if (!(flags & TrackSelection::AllTrackerLayers))
    return;

  foreach(Hit* hit, hits) {
    Cluster* cluster = static_cast<Cluster*>(hit);
    if (hit->detId() - hit->channel() == m_id)
      histogram(0)->Fill(cluster->hits().size());
  }
}

void ClusterLengthPlot::finalize()
{
}

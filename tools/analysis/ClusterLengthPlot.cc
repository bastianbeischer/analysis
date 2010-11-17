#include "ClusterLengthPlot.hh"

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

void ClusterLengthPlot::processEvent(const QVector<Hit*>& hits, Track* /*track*/, SimpleEvent* /*event*/)
{
  int nTrackerHits = 0;
  foreach(Hit* hit, hits)
    if (hit->type() == Hit::tracker)
      ++nTrackerHits;
  if (nTrackerHits != 8)
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

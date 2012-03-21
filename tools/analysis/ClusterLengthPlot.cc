#include "ClusterLengthPlot.hh"

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include <TH1D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "Cluster.hh"

ClusterLengthPlot::ClusterLengthPlot(Enums::AnalysisTopic topic, unsigned short id) :
  AnalysisPlot(topic),
  H1DPlot(),
  m_id(id)
{
  setTitle(QString("cluster length 0x%1").arg(m_id,0,16));
  TH1D* histogram = new TH1D(qPrintable(title()), "", 11, -0.5, 10.5);
  setAxisTitle("cluster length", "");
  histogram->GetXaxis()->SetNdivisions(520);
  addHistogram(histogram);

  addLatex(RootPlot::newLatex(0.65, 0.75));
  addLatex(RootPlot::newLatex(0.65, 0.70));

  addRequiredEventFlags(Enums::AllTrackerLayers);
}

ClusterLengthPlot::~ClusterLengthPlot()
{
}

void ClusterLengthPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  if (!track)
    return;

  const QVector<Hit*>::const_iterator endIt = event->clusters().end();
  for (QVector<Hit*>::const_iterator it = event->clusters().begin(); it != endIt; ++it) {
    Cluster* cluster = static_cast<Cluster*>(*it);
    if (cluster->elementId() == m_id)
      histogram()->Fill(cluster->hits().size());
  }
}

void ClusterLengthPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("mean = %1").arg(histogram()->GetMean())));
  latex(1)->SetTitle(qPrintable(QString("rms = %1").arg(histogram()->GetRMS())));
}

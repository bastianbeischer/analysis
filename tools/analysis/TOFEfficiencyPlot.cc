#include "TOFEfficiencyPlot.hh"
#include "BrokenLine.hh"

#include "TrackInformation.hh"
#include "Hit.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Constants.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLatex.h>

#include <QDebug>

TOFEfficiencyPlot::TOFEfficiencyPlot(unsigned short id)
  : AnalysisPlot(AnalysisPlot::EfficiencyTOF)
  , H2DPlot()
  , m_id(id)
  , m_updateCounter(0)
{
  setTitle(QString("tof efficiency 0x%1").arg(id, 0, 16));
  m_normalizationHistogram = new TH2I(qPrintable(title() + " normalization"),  "",
    20, -200, 200, 10, -25, 25);
  TH2D* histogram = new TH2D(qPrintable(title()), "",
    20, -200, 200, 10, -25, 25);
  histogram->GetXaxis()->SetTitle("y_{tracker} / mm");
  histogram->GetYaxis()->SetTitle("x_{tracker} / mm");
  histogram->GetZaxis()->SetTitle("efficiency");
  setHistogram(histogram);
  setDrawOption("LEGO2");
}

TOFEfficiencyPlot::~TOFEfficiencyPlot()
{
  delete m_normalizationHistogram;
}

void TOFEfficiencyPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  const QVector<Hit*>::const_iterator endIt = clusters.end();
  for (QVector<Hit*>::const_iterator it = clusters.begin(); it != endIt; ++it) {
    Hit* cluster = *it;
    if (cluster->type() == Hit::tof && (m_id & 0xFFFC) == cluster->detId()) {
      TOFCluster* tofCluster= static_cast<TOFCluster*>(cluster);
      double x = track->x(cluster->position().z()) - cluster->position().x();
      double y = track->y(cluster->position().z());
      if (qAbs(x) > Constants::tofBarWidth / 2. || qAbs(y) > Constants::tofBarLength / 2.)
        return;
      for (unsigned int i = 0; i < tofCluster->hits().size(); ++i) {
        TOFSipmHit* hit = static_cast<TOFSipmHit*>(tofCluster->hits()[i]);
        if (hit->detId() == m_id)
          histogram()->Fill(y, x);
      }
      m_normalizationHistogram->Fill(y, x);
    }
  }
}

void TOFEfficiencyPlot::update()
{
  ++m_updateCounter;
  if (m_updateCounter % 4 == 0) {
    AnalysisPlot::update();
    m_updateCounter = 0;
  }
}

void TOFEfficiencyPlot::finalize()
{
  histogram()->GetZaxis()->SetRangeUser(-0.25, 1.25);
  histogram()->Divide(m_normalizationHistogram);
}

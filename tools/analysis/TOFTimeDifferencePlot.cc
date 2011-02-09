#include "TOFTimeDifferencePlot.hh"
#include "BrokenLine.hh"

#include "TrackInformation.hh"
#include "Hit.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLatex.h>

#include <QDebug>

TOFTimeDifferencePlot::TOFTimeDifferencePlot(unsigned short id)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H2DPlot()
  , m_id(id)
  , m_updateCounter(0)
{
  setTitle(QString("tof time difference position 0x%1").arg(id, 0, 16));
  m_normalizationHistogram = new TH2I(qPrintable(title() + " normalization"),  "",
    20, -200, 200, 10, -25, 25);
  TH2D* histogram = new TH2D(qPrintable(title()), "",
    20, -200, 200, 10, -25, 25);
  histogram->GetXaxis()->SetTitle("y_{tracker} / mm");
  histogram->GetYaxis()->SetTitle("x_{tracker} / mm");
  histogram->GetZaxis()->SetTitle("(t_{left} - t_{right}) / ns");
  setHistogram(histogram);
}

TOFTimeDifferencePlot::~TOFTimeDifferencePlot()
{
  delete m_normalizationHistogram;
}

void TOFTimeDifferencePlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  foreach(Hit* cluster, clusters) {
    if (cluster->type() == Hit::tof && cluster->detId() - cluster->channel() == m_id) {
      TOFCluster* tofCluster= static_cast<TOFCluster*>(cluster);
      double t[4];
      for (int i = 0; i < 4; ++i)
        t[i] = -1;
      if (tofCluster->hits().size() != 4)
        return;
      for (unsigned int i = 0; i < tofCluster->hits().size(); ++i) {
        TOFSipmHit* hit = static_cast<TOFSipmHit*>(tofCluster->hits()[i]);
        if (hit->position().y() < 0) {
          if (hit->position().x() < cluster->position().x()) {
            Q_ASSERT(t[0] < 0);
            t[0] = hit->startTime() - hit->photonTravelTime();
          } else {
            Q_ASSERT(t[1] < 0);
            t[1] = hit->startTime() - hit->photonTravelTime();
          }
        } else {
          if (hit->position().x() < cluster->position().x()) {
            Q_ASSERT(t[2] < 0);
            t[2] = hit->startTime() - hit->photonTravelTime();
          } else {
            Q_ASSERT(t[3] < 0);
            t[3] = hit->startTime() - hit->photonTravelTime();
          }
        }
      }
      double dt = (t[0]+t[1])/2. - (t[2]+t[3])/2.;
      if (qAbs(dt) < 10) {
        double bending = track->x(cluster->position().z()) - cluster->position().x();
        double nonBending = track->y(cluster->position().z());
        m_normalizationHistogram->Fill(nonBending, bending);
        histogram()->Fill(nonBending, bending, dt);
      }
      return;
    }
  }
}

void TOFTimeDifferencePlot::update()
{
  ++m_updateCounter;
  if (m_updateCounter % 4 == 0) {
    AnalysisPlot::update();
    m_updateCounter = 0;
  }
}

void TOFTimeDifferencePlot::draw(TCanvas* canvas)
{
  H2DPlot::draw(canvas);
  histogram()->Draw("lego2");
}

void TOFTimeDifferencePlot::finalize()
{
  histogram()->GetZaxis()->SetRangeUser(-3, 3);
  histogram()->Divide(m_normalizationHistogram);
}

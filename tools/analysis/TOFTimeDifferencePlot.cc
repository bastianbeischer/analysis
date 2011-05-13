#include "TOFTimeDifferencePlot.hh"
#include "BrokenLine.hh"

#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
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

TOFTimeDifferencePlot::TOFTimeDifferencePlot(unsigned short id)
  : AnalysisPlot(AnalysisPlot::CalibrationTOF)
  , H2DPlot()
  , m_id(id)
  , m_updateCounter(0)
{
  setTitle(QString("tof time difference position 0x%1").arg(id, 0, 16));
  m_normalizationHistogram = new TH2I(qPrintable(title() + " normalization"),  "",
    20, -200, 200, 10, -25, 25);
  TH2D* histogram = new TH2D(qPrintable(title()), "",
    20, -200, 200, 10, -25, 25);
  setAxisTitle("y_{tracker} / mm", "x_{tracker} / mm", "(t_{left} - t_{right}) / ns");
  addHistogram(histogram);
  setDrawOption(SURF1);
}

TOFTimeDifferencePlot::~TOFTimeDifferencePlot()
{
  delete m_normalizationHistogram;
}

void TOFTimeDifferencePlot::processEvent(const QVector<Hit*>& clusters, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;

  const QVector<Hit*>::const_iterator endIt = clusters.end();
  for (QVector<Hit*>::const_iterator it = clusters.begin(); it != endIt; ++it) {
    Hit* cluster = *it;
    if (cluster->type() == Hit::tof && cluster->detId() - cluster->channel() == m_id) {
      TOFCluster* tofCluster= static_cast<TOFCluster*>(cluster);
      double t[4];
      for (int i = 0; i < 4; ++i)
        t[i] = -2e6;
      Q_ASSERT(tofCluster->hits().size() >=  3);
      for (unsigned int i = 0; i < tofCluster->hits().size(); ++i) {
        TOFSipmHit* hit = static_cast<TOFSipmHit*>(tofCluster->hits()[i]);
        if (hit->position().y() < 0) {
          if (hit->position().x() < cluster->position().x()) {
            Q_ASSERT(t[0] < -1e6);
            t[0] = hit->startTime() - hit->photonTravelTime();
          } else {
            Q_ASSERT(t[1] < -1e6);
            t[1] = hit->startTime() - hit->photonTravelTime();
          }
        } else {
          if (hit->position().x() < cluster->position().x()) {
            Q_ASSERT(t[2] < -1e6);
            t[2] = hit->startTime() - hit->photonTravelTime();
          } else {
            Q_ASSERT(t[3] < -1e6);
            t[3] = hit->startTime() - hit->photonTravelTime();
          }
        }
      }
      for (int i = 0; i < 4; ++i)
        if (t[i] < -1e6)
          t[i] = 2e6;
      double dt = (t[0] < t[1] ? t[0] : t[1]) - (t[2] < t[3] ? t[2] : t[3]);
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

void TOFTimeDifferencePlot::finalize()
{
  zAxis()->SetRangeUser(-1, 1);
  for (int binX = 1; binX < m_normalizationHistogram->GetXaxis()->GetNbins(); ++binX)
    for (int binY = 1; binY < m_normalizationHistogram->GetYaxis()->GetNbins(); ++binY) {
      double value = Constants::sigmaSipm / sqrt(m_normalizationHistogram->GetBinContent(binX, binY));
      histogram()->SetBinError(binX, binY, value);
    }
  histogram()->Divide(m_normalizationHistogram);
}

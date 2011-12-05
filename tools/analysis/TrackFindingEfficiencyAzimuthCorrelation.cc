#include "TrackFindingEfficiencyAzimuthCorrelation.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Plotter.hh"
#include "RootQtWidget.hh"
#include "Hit.hh"
#include "Helpers.hh"

#include <TH2D.h>
#include <TLatex.h>
#include <TVector3.h>

#include <QVector>
#include <QDebug>

#include <cmath>
#include <vector>

TrackFindingEfficiencyAzimuthCorrelation::TrackFindingEfficiencyAzimuthCorrelation(EfficiencyCorrectionSettings::FoldingType type)
  : AnalysisPlot(Enums::MiscellaneousTracker)
  , H2DPlot()
  , m_reconstructed(0)
  , m_total(0)
{
  const QString& htitle = QString("Track finding efficiency azimuth correlation - ") + EfficiencyCorrectionSettings::instance()->foldingTypeName(type);
  int nBinsData = EfficiencyCorrectionSettings::numberOfBins(type);
  setTitle(htitle);
  const double minData = 0.1;
  const double maxData = 20;
  QVector<double> axis = Helpers::logBinning(nBinsData, minData, maxData);
  int axisSize = axis.size()*2;
  for (int i = 0; i < axisSize; i+=2) {
    double value = axis.at(i);
    axis.prepend(-value);
  }
  const int nBins = axis.size() - 1;
  const int nBinsAzimuth = 20;
  const double xMinAzimuth = -180;
  const double xMaxAzimuth = 180;
  TH2D* histogram = new TH2D("reconstruction efficiency", "", nBins, axis.constData(), nBinsAzimuth, xMinAzimuth, xMaxAzimuth);
  histogram->Sumw2();
  int low = histogram->GetXaxis()->FindBin(0.5);
  int up = histogram->GetXaxis()->FindBin(5);
  histogram->GetXaxis()->SetRange(low, up);
  histogram->GetXaxis()->SetTitleOffset(1.2);
  histogram->GetYaxis()->SetTitleOffset(1.2);
  histogram->GetZaxis()->SetTitleOffset(1.2);
//  Plotter::rootWidget()->GetCanvas()->SetRightMargin(0.18);

  setAxisTitle("mc rigidity / GV", "mc azimuth / degree", "efficiency");
  addHistogram(histogram);

  m_reconstructed = new TH2D("reconstructed", "", nBins, axis.constData(), nBinsAzimuth, xMinAzimuth, xMaxAzimuth);
  m_reconstructed->Sumw2();
  m_total = new TH2D("total", "", nBins, axis.constData(), nBinsAzimuth, xMinAzimuth, xMaxAzimuth);
  m_total->Sumw2();

  addLatex(RootPlot::newLatex(.3, .85));
}


TrackFindingEfficiencyAzimuthCorrelation::~TrackFindingEfficiencyAzimuthCorrelation()
{
  delete m_reconstructed;
  delete m_total;
}

void TrackFindingEfficiencyAzimuthCorrelation::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
{
  bool fillHistogram = false;
  double rigidity = 0;
  double azimuth = 0;
  if (event->contentType() == SimpleEvent::MonteCarlo) {
    if (!event->MCInformation()->primary()->isInsideMagnet())
      return;
    //Todo: albedo handling
    int mcPdgId = event->MCInformation()->primary()->pdgID;
    Particle mcParticle(mcPdgId);
    rigidity = event->MCInformation()->primary()->initialMomentum.Mag() / mcParticle.charge();
    azimuth = event->MCInformation()->primary()->azimuthAngle() * 180. / M_PI;;
    if (!isTriggerEvent(QVector<Hit*>::fromStdVector(event->hits())))
      return;
    fillHistogram = true;
  }

  if (fillHistogram)
    m_total->Fill(rigidity, azimuth);

  const Track* track = particle->track();
  if (!track || !track->fitGood())
    return;
  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & (ParticleInformation::Chi2Good)))
    return;

  if (fillHistogram)
    m_reconstructed->Fill(rigidity, azimuth);
}

void TrackFindingEfficiencyAzimuthCorrelation::update()
{
  for (int i = 0; i < m_total->GetNbinsX(); ++i) {
    for (int j = 0; j < m_total->GetNbinsY(); ++j) {
      int reconstructed = m_reconstructed->GetBinContent(i+1, j+1);
      int total = m_total->GetBinContent(i+1, j+1);
      double efficiency = 0;
      double efficiencyError = 0;
      if (total != 0) {
        efficiency = double(reconstructed) / double(total);
        efficiencyError =  sqrt(efficiency * (1 - efficiency) / double(total));
      }
      histogram()->SetBinContent(i+1, j+1, efficiency);
      histogram()->SetBinError(i+1, j+1, efficiencyError);
    }
  }
}

bool TrackFindingEfficiencyAzimuthCorrelation::isTriggerEvent(const QVector<Hit*>& clusters)
{
  const QVector<Hit*>::const_iterator endIt = clusters.end();
  bool hitUpperTof = false;
  bool hitLowerTof = false;
  for (QVector<Hit*>::const_iterator clusterIt = clusters.begin(); clusterIt != endIt; ++clusterIt) {
    Hit* hit = *clusterIt;
    if (hit->type() == Hit::tof) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(hit);
      if (tofCluster->position().z() > 0)
        hitUpperTof = true;
      if (tofCluster->position().z() < 0)
        hitLowerTof = true;
    }
  }
  return (hitUpperTof && hitLowerTof);
}

void TrackFindingEfficiencyAzimuthCorrelation::finalize() {
}

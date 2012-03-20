#include "TrackFindingEfficiencyZenithCorrelation.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "Plotter.hh"
#include "RootQtWidget.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Hit.hh"
#include "Helpers.hh"
#include "AnalyzedEvent.hh"
#include "ParticleProperties.hh"
#include "ParticleDB.hh"

#include <TH2D.h>
#include <TLatex.h>
#include <TVector3.h>

#include <QVector>
#include <QDebug>

#include <cmath>
#include <vector>

TrackFindingEfficiencyZenithCorrelation::TrackFindingEfficiencyZenithCorrelation(bool fineBinned)
  : AnalysisPlot(Enums::MiscellaneousTracker)
  , H2DPlot()
  , m_reconstructed(0)
  , m_total(0)
{
  QString title = "track finding efficiency zenith correlation";
  if (fineBinned)
    title+= " fine";
  setTitle(title);

  QVector<double> axis = Helpers::rigidityBinning(fineBinned);
  int axisSize = axis.size()*2;
  for (int i = 0; i < axisSize; i+=2) {
    double value = axis.at(i);
    axis.prepend(-value);
  }
  const int nBins = axis.size() - 1;
  const int nBinsZenith = 200;
  const double xMinZenith = 0;
  const double xMaxZenith = 1;
  TH2D* histogram = new TH2D("reconstruction efficiency", "", nBins, axis.constData(), nBinsZenith, xMinZenith, xMaxZenith);
  histogram->Sumw2();
  int low = histogram->GetXaxis()->FindBin(0.5);
  int up = histogram->GetXaxis()->FindBin(5);
  histogram->GetXaxis()->SetRange(low, up);
  histogram->GetXaxis()->SetTitleOffset(1.2);
  histogram->GetYaxis()->SetTitleOffset(1.2);
  histogram->GetZaxis()->SetTitleOffset(1.2);

  int lowY = histogram->GetYaxis()->FindBin(0.9);
  int upY = histogram->GetYaxis()->FindBin(1.);
  histogram->GetYaxis()->SetRange(lowY, upY);

  setAxisTitle("mc rigidity / GV", "cos(mc zenith)", "efficiency");
  addHistogram(histogram);

  m_reconstructed = new TH2D("reconstructed", "", nBins, axis.constData(), nBinsZenith, xMinZenith, xMaxZenith);
  m_reconstructed->Sumw2();
  m_total = new TH2D("total", "", nBins, axis.constData(), nBinsZenith, xMinZenith, xMaxZenith);
  m_total->Sumw2();

  addLatex(RootPlot::newLatex(.3, .85));
}


TrackFindingEfficiencyZenithCorrelation::~TrackFindingEfficiencyZenithCorrelation()
{
  delete m_reconstructed;
  delete m_total;
}

void TrackFindingEfficiencyZenithCorrelation::processEvent(const AnalyzedEvent* event)
{
  bool fillHistogram = false;
  double rigidity = 0;
  double zenith = 0;
  if (event->simpleEvent()->contentType() == SimpleEvent::MonteCarlo) {
    if (!event->simpleEvent()->MCInformation()->primary()->isInsideMagnet())
      return;
    //Todo: albedo handling
    int mcPdgId = event->simpleEvent()->MCInformation()->primary()->pdgID;
    const ParticleProperties* mcParticle = ParticleDB::instance()->lookupPdgId(mcPdgId);
    rigidity = event->simpleEvent()->MCInformation()->primary()->initialMomentum.Mag() / mcParticle->charge();
    zenith = event->simpleEvent()->MCInformation()->primary()->zenithAngle();
    if (!isTriggerEvent(QVector<Hit*>::fromStdVector(event->simpleEvent()->hits())))
      return;
    fillHistogram = true;
  }

  if (fillHistogram)
    m_total->Fill(rigidity, cos(zenith));

  const Track* track = event->particle()->track();
  if (!track || !track->fitGood())
    return;
  Enums::EventFlags flags = event->particle()->information()->eventFlags();
  if (!(flags & (Enums::Chi2Good)))
    return;

  if (fillHistogram)
    m_reconstructed->Fill(rigidity, cos(zenith));
}

void TrackFindingEfficiencyZenithCorrelation::update()
{
  if (!m_total->GetEntries())
    return;
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

bool TrackFindingEfficiencyZenithCorrelation::isTriggerEvent(const QVector<Hit*>& clusters)
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

void TrackFindingEfficiencyZenithCorrelation::finalize() {
}

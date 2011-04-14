#include "MCRigidityResolution.hh"

#include <TH1D.h>
#include <TF1.h>
#include <TLatex.h>

#include "SimpleEvent.hh"
#include "Track.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"

#include "TF1.h"

MCRigidityResolution::MCRigidityResolution(int pdgID)
  : AnalysisPlot(AnalysisPlot::MonteCarloTracker)
  , H1DPlot()
  , m_pdgID(pdgID)
  , m_rigidityRangeLower(0.05)
  , m_rigidityRangeUppper(10.05)
  , m_numberOfBins(100)
{
  setTitle("rigidity resolution for " + QString::number(m_pdgID));

  TH1D* hist = new TH1D(qPrintable(title()), qPrintable(title())
                        , m_numberOfBins
                        , m_rigidityRangeLower
                        , m_rigidityRangeUppper);
  setAxisTitle("R / GeV","resolution");
  addHistogram(hist);

  TF1 expectedRes(qPrintable("expected resolution for " + title()), "sqrt(([2]*x)**2 + ([3]*sqrt(1+[0]*[0]/([1]*[1]*x*x)))**2)");
  //TODO use particle class and its mass
  switch (qAbs(m_pdgID))
  {
  case 2212:
    expectedRes.SetParameters(1,1,0.07766,0.237);
    break;
  case 11:
    expectedRes.SetParameters(0.000511,1,0.07698,0.2583);
    break;
  case 1000020040:
    expectedRes.SetParameters(4,2,0.07698,0.2583);
    break;
  default:
    expectedRes.SetParameters(1,1,0.07766,0.237);
    break;
  }

  for (int i = 1; i <= hist->GetNbinsX(); ++i) {
    QString histTitle = QString("resolutionhist_%1_%2").arg(m_pdgID).arg(i);
    double mcRig = hist->GetBinCenter(i);
    double inverseRigRange = expectedRes.Eval(mcRig)*5;
    int resolutionBins = 100;
    m_resolutionHistos.insert(i, new TH1D(qPrintable(histTitle)
                                         ,qPrintable(histTitle)
                                         , resolutionBins, -inverseRigRange, inverseRigRange));

  }
}

MCRigidityResolution::~MCRigidityResolution()
{
  qDeleteAll(m_resolutionHistos);
}


void MCRigidityResolution::processEvent(const QVector<Hit*>& /*hits*/, Particle* particle, SimpleEvent* event)
{
  //only accept mc events:
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;

  //get track
  Track* track = particle->track();

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  //check if track fits chi2 is ok
  if (!(particle->information()->flags() & ParticleInformation::Chi2Good))
    return;

  //check if track was inside of magnet
  if (!(particle->information()->flags() & ParticleInformation::InsideMagnet))
    return;

  //only use selected pdgID
  if (event->MCInformation()->primary()->pdgID != m_pdgID)
    return;


  //get mc rigidity
  double mcMom = event->MCInformation()->primary()->initialMomentum.Mag();

  double mcRigidity = mcMom;

  //get the reconstructed momentum
  double rigidity = track->rigidity(); //GeV

  double inverseDifference = 1./rigidity - 1./mcRigidity; // 1 /GV

  //get bin
  int bin = histogram()->FindBin(mcRigidity);


  if (m_resolutionHistos.contains(bin)) {
    TH1D* hist = m_resolutionHistos.value(bin);
    hist->Fill(inverseDifference * mcRigidity); //already multiply here with mcRigidity !!!!
  }
}

void MCRigidityResolution::update()
{
  histogram()->Reset();

  QMap<int, TH1D*>::const_iterator i;
  for (i = m_resolutionHistos.constBegin(); i != m_resolutionHistos.constEnd(); ++i) {
    TH1D* hist = i.value();
    double inverseRMS = hist->GetRMS();
    double inverseRMSError = hist->GetRMSError();
    double rigidityRes = inverseRMS ;
    double rigidityResErr = inverseRMSError ;

    histogram()->SetBinContent(i.key(), rigidityRes);
    histogram()->SetBinError(i.key(), rigidityResErr);
  }


}

void MCRigidityResolution::finalize()
{
  update();
}

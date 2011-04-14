#include "MCRigidityResolution.hh"

#include <TH1D.h>
#include <TF1.h>
#include <TLatex.h>

#include "SimpleEvent.hh"
#include "Track.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "ParticleDB.hh"
#include "ParticleProperties.hh"
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
  const ParticleProperties* partProps = ParticleDB::instance()->lookupPdgId(m_pdgID);
  QString particleName = partProps->name();

  setTitle("rigidity resolution for " + particleName);

  TH1D* hist = new TH1D(qPrintable(title()), qPrintable(title())
                        , m_numberOfBins
                        , m_rigidityRangeLower
                        , m_rigidityRangeUppper);
  setAxisTitle("R / GV","#sigma_{R}/R");
  hist->GetYaxis()->SetRangeUser(0,1.5);
  addHistogram(hist);

  TF1* expectedRes = new TF1(qPrintable("expected resolution for " + title()), "sqrt(([2]*[1]*x)**2 + ([3]*sqrt(1+[0]*[0]/([1]*[1]*x*x)))**2)", 0, 20);
  expectedRes->SetParNames("mass/GeV", "abs(charge)", "a", "b");

  expectedRes->FixParameter(0, partProps->mass());
  expectedRes->FixParameter(1, qAbs(partProps->charge()));

  switch (partProps->type())
  {
  case Particle::Proton: //case Particle::AntiProton:
    expectedRes->SetParameter(2, 0.077);
    expectedRes->SetParameter(3, 0.3175);
    break;
  case Particle::Electron: case Particle::Positron:
    expectedRes->SetParameter(2, 0.07627);
    expectedRes->SetParameter(3, 0.2349);
    break;
  case Particle::Helium:
    expectedRes->SetParameter(2, 0.04195);
    expectedRes->SetParameter(3, 0.3024);
    break;
  default:
    expectedRes->SetParameter(2, 0.077);
    expectedRes->SetParameter(3, 0.3175);
    break;
  }

  for (int i = 1; i <= hist->GetNbinsX(); ++i) {
    QString histTitle = QString("resolutionhist_%1_%2").arg(particleName).arg(i);
    double mcRig = hist->GetBinCenter(i);
    double inverseRigRange = expectedRes->Eval(mcRig)*5;
    int resolutionBins = 100;
    m_resolutionHistos.insert(i, new TH1D(qPrintable(histTitle)
                                         , qPrintable(histTitle)
                                         , resolutionBins, -inverseRigRange, inverseRigRange));

  }

  TF1* fittedRes = new TF1(*expectedRes);

  expectedRes->SetLineColor(kBlack);
  expectedRes->SetLineStyle(kDashed);
  fittedRes->SetLineColor(kRed);
  addFunction(expectedRes);
  addFunction(fittedRes);

  TLatex* ltx = new TLatex(5,0.3,"#sigma_{R}/R = #sqrt{(a*c*R)^{2}+(b/#beta)^{2}}");
  ltx->SetTextSizePixels(50);
  TLatex* ltxa = new TLatex(6,0.2,qPrintable("a_{old} = " + QString::number(expectedRes->GetParameter("a"))));
  ltxa->SetTextSizePixels(50);
  TLatex* ltxb = new TLatex(6,0.1,qPrintable("b_{old} = " + QString::number(expectedRes->GetParameter("b"))));
  ltxb->SetTextSizePixels(50);

  addLatex(ltx);
  addLatex(ltxa);
  addLatex(ltxb);
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

  double mcRigidity = m_pdgID==1000020040 ? mcMom/2. : mcMom;

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
    TF1* fit = new TF1("gausfit","gaus");
    hist->Fit(fit,"QN");
    double inverseSigma = fit->GetParameter(2);
    double inverseSigmaErr = fit->GetParError(2);
    double rigidityRes = inverseSigma ;
    double rigidityResErr = inverseSigmaErr ;

    histogram()->SetBinContent(i.key(), rigidityRes);
    histogram()->SetBinError(i.key(), rigidityResErr);
  }

  histogram()->Fit(function(1),"q0");

}

void MCRigidityResolution::finalize()
{
  update();
}

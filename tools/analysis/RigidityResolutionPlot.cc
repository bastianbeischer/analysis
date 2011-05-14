#include "RigidityResolutionPlot.hh"

#include <QHBoxLayout>
#include <QLabel>

#include <TH1D.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TLatex.h>

#include "SimpleEvent.hh"
#include "Track.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "ParticleDB.hh"
#include "ParticleProperties.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "RootQtWidget.hh"

#include "StringSpinBox.hh"

#include "TF1.h"

RigidityResolutionPlot::RigidityResolutionPlot(AnalysisPlot::Topic topic, int pdgID)
  : AnalysisPlot(topic)
  , H1DPlot()
  , m_particle(ParticleDB::instance()->lookupPdgId(pdgID))
  , m_rigidityRangeLower(-0.025)
  , m_rigidityRangeUppper(10.025)
  , m_numberOfBins(201)
  , m_rigDistributionWidget(new RootQtWidget)
{

  Q_ASSERT(m_particle);

  QString particleName = m_particle->name();

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

  expectedRes->FixParameter(0, m_particle->mass());
  expectedRes->FixParameter(1, qAbs(m_particle->charge()));

  switch (m_particle->type())
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
    double genRig = hist->GetBinCenter(i);
    QString histTitle = QString("rigidity resolution for %1 at %2 GV").arg(particleName).arg(genRig);
    double inverseRigRange = expectedRes->Eval(genRig)*5;
    int resolutionBins = 100;
    m_resolutionHistos.insert(i, new TH1D(qPrintable(histTitle)
                                         , qPrintable(histTitle+";1/R;entries")
                                         , resolutionBins, 1.-inverseRigRange, 1.+inverseRigRange));

  }

  TF1* fittedRes = new TF1(*expectedRes);

  expectedRes->SetLineColor(kBlack);
  expectedRes->SetLineStyle(kDashed);
  fittedRes->SetLineColor(kRed);
  addFunction(expectedRes);
  addFunction(fittedRes);

  TLatex* ltx = new TLatex(5,0.3,"#sigma_{R}/R = #sqrt{(azR)^{2}+(b/#beta)^{2}}");
  ltx->SetTextSizePixels(50);
  TLatex* ltxa = new TLatex(6,0.2,qPrintable("a_{old} = " + QString::number(expectedRes->GetParameter("a"))));
  ltxa->SetTextSizePixels(50);
  TLatex* ltxb = new TLatex(6,0.1,qPrintable("b_{old} = " + QString::number(expectedRes->GetParameter("b"))));
  ltxb->SetTextSizePixels(50);

  addLatex(ltx);
  addLatex(ltxa);
  addLatex(ltxb);

  setSecondaryWidget(m_rigDistributionWidget);
}

RigidityResolutionPlot::~RigidityResolutionPlot()
{
  qDeleteAll(m_resolutionHistos);
}

void RigidityResolutionPlot::positionChanged(double posX, double)
{
  loadRigHisto(posX);
}

void RigidityResolutionPlot::loadRigHisto(double rig)
{
  RootQtWidget* widget = m_rigDistributionWidget;
  if (widget->isVisible()) {
    int bin = histogram()->FindBin(rig);
    loadRigHisto(bin);
  }
}
void RigidityResolutionPlot::loadRigHisto(int bin)
{
  RootQtWidget* widget = m_rigDistributionWidget;
  if (widget->isVisible()) {
    if (! m_resolutionHistos.contains(bin))
      return;

    TH1D* hist = m_resolutionHistos.value(bin);

    TVirtualPad* prevPad = gPad;
    TCanvas* can = m_rigDistributionWidget->GetCanvas();
    can->cd();
    can->Clear();
    hist->Draw();
    TF1* fit = hist->GetFunction("gaus");
    if (fit)
      fit->Draw("same");
    can->Modified();
    can->Update();
    prevPad->cd();
  }
}

void RigidityResolutionPlot::processEvent(const QVector<Hit*>& /*hits*/, Particle* particle, SimpleEvent* event)
{
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

  // get the reconstructed momentum
  double rigidity = track->rigidity(); //GeV

  // and the reference
  double refRigidity = referenceRigidity(event);

  //get bin and fill
  int bin = histogram()->FindBin(qAbs(refRigidity));
  if (m_resolutionHistos.contains(bin)) {
    TH1D* hist = m_resolutionHistos.value(bin);
    hist->Fill(refRigidity/rigidity);
  }
}

void RigidityResolutionPlot::update()
{
  histogram()->Reset();

  QMap<int, TH1D*>::const_iterator i;
  for (i = m_resolutionHistos.constBegin(); i != m_resolutionHistos.constEnd(); ++i) {
    TH1D* hist = i.value();
    if (hist->GetEntries() < 10)
      continue;
    //double axisMax = hist->GetXaxis()->GetXmax();
    //TF1* fit = new TF1("gausfit","gaus", -axisMax, axisMax);
    TFitResultPtr r = hist->Fit("gaus","Q0SR",0, 0, 1.5);
    double inverseSigma = r->Parameter(2);
    double inverseSigmaErr = r->ParError(2);
    double rigidityRes = inverseSigma;
    double rigidityResErr = inverseSigmaErr ;

    histogram()->SetBinContent(i.key(), rigidityRes);
    histogram()->SetBinError(i.key(), rigidityResErr);
  }

  if (histogram()->GetEntries() > 3)
    histogram()->Fit(function(1),"q0");
}

void RigidityResolutionPlot::finalize()
{
  update();
  //saveHistos();
}

void RigidityResolutionPlot::saveHistos()
{
  QMap<int, TH1D*>::const_iterator i;
  for (i = m_resolutionHistos.constBegin(); i != m_resolutionHistos.constEnd(); ++i) {
    TH1D* hist = i.value();
    if (hist->GetEntries() < 10)
      continue;
    double genRig = histogram()->GetBinCenter(i.key());
    hist->SaveAs(qPrintable(QString("rigres_for_%1_at_%2GV.root").arg(m_particle->name()).arg(genRig)));
  }
}

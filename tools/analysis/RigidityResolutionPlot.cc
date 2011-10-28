#include "RigidityResolutionPlot.hh"
#include "Helpers.hh"
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
#include "Constants.hh"

#include <TH1D.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TLatex.h>
#include <TF1.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

RigidityResolutionPlot::RigidityResolutionPlot(Enums::AnalysisTopic topic, Enums::Particle type)
  : AnalysisPlot(topic)
  , H1DPlot()
  , m_particleProperties(type)
  , m_rigDistributionWidget(new RootQtWidget)
  , m_numberOfBins(200)
  , m_min(0.025)
  , m_max(10.025)
  , m_trackerResolutionHistograms(m_numberOfBins, 0)
  , m_tofResolutionHistograms(m_numberOfBins, 0)
  , m_weightedMeanResolutionHistograms(m_numberOfBins, 0)
  , m_likelihoodResolutionHistograms(m_numberOfBins, 0)
{
  QString particleName = m_particleProperties.name();
  QString title = "rigidity resolution " + particleName;

  addHistogram(new TH1D(qPrintable("tracker " + title), "", m_numberOfBins, m_min, m_max));
  addHistogram(new TH1D(qPrintable("TOF " + title), "", m_numberOfBins, m_min, m_max));
  addHistogram(new TH1D(qPrintable("weighted mean " + title), "", m_numberOfBins, m_min, m_max));
  addHistogram(new TH1D(qPrintable("likelihood " + title), "", m_numberOfBins, m_min, m_max));

  TF1* trackerFunction = newTrackerResolutionFunction("expected tracker resolution " + particleName);
  addFunction(trackerFunction);
  TF1* tofFunction = newTofResolutionFunction("expected TOF resolution " + particleName);
  addFunction(tofFunction);
  addFunction(newWeightedMeanResolutionFunction("expected weighted mean resolution " + particleName));

  for (int bin = 1; bin <= m_numberOfBins; ++bin) {
    double genRig = xAxis()->GetBinCenter(bin);
    QString histTitle = QString("rigidity resolution for %1 at %2 GV").arg(particleName).arg(genRig);
    double inverseRigRange = trackerFunction->Eval(genRig) * 5;
    int resolutionBins = 100;
    m_trackerResolutionHistograms[bin-1] = new TH1D(qPrintable("tracker " + histTitle), ";1/R;entries",
      resolutionBins, 1.-inverseRigRange, 1.+inverseRigRange);
    m_tofResolutionHistograms[bin-1] = new TH1D(qPrintable("TOF " + histTitle), ";1/R;entries",
      resolutionBins, 1.-inverseRigRange, 1.+inverseRigRange);
    m_weightedMeanResolutionHistograms[bin-1] = new TH1D(qPrintable("weighted mean " + histTitle), ";1/R;entries",
      resolutionBins, 1.-inverseRigRange, 1.+inverseRigRange);
    m_likelihoodResolutionHistograms[bin-1] = new TH1D(qPrintable("likelihood " + histTitle), ";1/R;entries",
      resolutionBins, 1.-inverseRigRange, 1.+inverseRigRange);
  }

  TLatex* ltx = 0;
  ltx = newLatex(0.6, 0.35);
  ltx->SetTitle("#sigma_{R}/R = aR/GV  #oplus b/#beta");
  ltx->SetTextSizePixels(40);
  addLatex(ltx);
  ltx = newLatex(0.6, 0.27);
  ltx->SetTitle(qPrintable(QString("%1: a = %2, b = %3").arg(m_particleProperties.name())
    .arg(trackerFunction->GetParameter("a"), 0, 'f', 3).arg(trackerFunction->GetParameter("b"), 0, 'f', 3)));
  ltx->SetTextSizePixels(40);
  addLatex(ltx);
  ltx = newLatex(0.6, 0.19);
  ltx->SetTitle(qPrintable(QString("#sigma_{TOF} = %1 ns").arg(tofFunction->GetParameter("sigma"), 0, 'f', 2)));
  ltx->SetTextSizePixels(40);
  addLatex(ltx);

  setTitle(title);
  setAxisTitle("R / GV", "#sigma_{R}/R");
  yAxis()->SetRangeUser(0, 1.5);

  setSecondaryWidget(m_rigDistributionWidget);
}

RigidityResolutionPlot::~RigidityResolutionPlot()
{
  qDeleteAll(m_trackerResolutionHistograms);
}

TF1* RigidityResolutionPlot::newTrackerResolutionFunction(const QString& name) const
{
  TF1* f = new TF1(qPrintable(name), this, &RigidityResolutionPlot::trackerResolution,
    m_min, m_max, 2, "RigidityResolutionPlot", "trackerResolution");
  f->SetParNames("a", "b");
  switch (m_particleProperties.type()) {
    case Enums::Proton: /*case Enums::AntiProton*/ f->SetParameters(0.077, 0.3175); break;
    case Enums::Electron: case Enums::Positron: f->SetParameters(0.07627, 0.2349); break;
    case Enums::Helium: f->SetParameters(0.04195, 0.3024); break;
    case Enums::PiPlus: case Enums::PiMinus: f->SetParameters(0.0759, 0.2356); break;
    default: f->SetParameters(0.077, 0.3175, 0.40); break;
  }
  return f;
}

TF1* RigidityResolutionPlot::newTofResolutionFunction(const QString& name) const
{
  TF1* f = new TF1(qPrintable(name), this, &RigidityResolutionPlot::tofResolution,
    m_min, m_max, 1, "RigidityResolutionPlot", "tofResolution");
  f->SetParNames("sigma");
  f->SetParameter(0, 0.40);
  return f;
}

TF1* RigidityResolutionPlot::newWeightedMeanResolutionFunction(const QString& name) const
{
  TF1* f = new TF1(qPrintable(name), this, &RigidityResolutionPlot::weightedMeanResolution,
    m_min, m_max, 3, "RigidityResolutionPlot", "weightedMeanResolution");
  f->SetParNames("a", "b", "sigma");
  switch (m_particleProperties.type()) {
    case Enums::Proton: /*case Enums::AntiProton*/ f->SetParameters(0.077, 0.3175, 0.40); break;
    case Enums::Electron: case Enums::Positron: f->SetParameters(0.07627, 0.2349, 0.40); break;
    case Enums::Helium: f->SetParameters(0.04195, 0.3024, 0.40); break;
    case Enums::PiPlus: case Enums::PiMinus: f->SetParameters(0.0759, 0.2356, 0.40); break;
    default: f->SetParameters(0.077, 0.3175, 0.40); break;
  }
  return f;
}

double RigidityResolutionPlot::trackerResolution(double* x, double* p) const
{
  KineticVariable variable(m_particleProperties.type(), Enums::AbsoluteRigidity, x[0]);
  return Helpers::addQuad(p[0]*variable.rigidity(), p[1]/variable.beta());
}

double RigidityResolutionPlot::tofResolution(double* x, double* timeResolution) const
{
  double distance = Constants::upperTofPosition - Constants::lowerTofPosition;
  double sigmaInverseBeta = Constants::speedOfLight / distance * timeResolution[0];
  double m = m_particleProperties.mass();
  double p = m_particleProperties.charge() * x[0];
  return p / (m*m) * Helpers::addQuad(m, p) * sigmaInverseBeta;
}

double RigidityResolutionPlot::weightedMeanResolution(double* x, double* p) const
{
  return 1./Helpers::addQuad(1./trackerResolution(x, &p[0]), 1./tofResolution(x, &p[2]));
}

void RigidityResolutionPlot::positionChanged(double posX, double)
{
  loadRigHisto(posX);
}

void RigidityResolutionPlot::loadRigHisto(double rig)
{
  RootQtWidget* widget = m_rigDistributionWidget;
  if (widget->isVisible()) {
    int bin = histogram(0)->FindBin(rig);
    loadRigHisto(bin);
  }
}

void RigidityResolutionPlot::loadRigHisto(int bin)
{
  RootQtWidget* widget = m_rigDistributionWidget;
  if (widget->isVisible()) {
    if (bin < 1 || m_numberOfBins < bin)
      return;

    TH1D* hist = m_trackerResolutionHistograms[bin-1];

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

void RigidityResolutionPlot::processEvent(const AnalyzedEvent* event)
{
  Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet | ParticleInformation::BetaGood))
    return;
  if (event->flagsSet(ParticleInformation::MagnetCollision))
    return;

  double refRigidity = referenceRigidity(event);
  int bin = histogram(0)->FindBin(qAbs(refRigidity));
  if (0 < bin  && bin <= m_numberOfBins) {
    m_trackerResolutionHistograms[bin-1]->Fill(refRigidity / track->rigidity());
    m_tofResolutionHistograms[bin-1]->Fill(refRigidity / KineticVariable(m_particleProperties.type(), Enums::Beta, event->particle()->beta()).rigidity());
  }
}

void RigidityResolutionPlot::update()
{
  histogram(0)->Reset();

  for (int bin = 1; bin <= m_numberOfBins; ++bin) {
    TH1D* hist = m_trackerResolutionHistograms[bin-1];
    if (hist->GetEntries() < 10)
      continue;
    //double axisMax = hist->GetXaxis()->GetXmax();
    //TF1* fit = new TF1("gausfit","gaus", -axisMax, axisMax);
    double mean = hist->GetMean();
    double rms = hist->GetRMS();
    TFitResultPtr r = hist->Fit("gaus", "Q0SR", 0, mean-2*rms, mean+2*rms);
    double inverseSigma = r->Parameter(2);
    double inverseSigmaErr = r->ParError(2);
    double rigidityRes = inverseSigma;
    double rigidityResErr = inverseSigmaErr;

    histogram(0)->SetBinContent(bin, rigidityRes);
    histogram(0)->SetBinError(bin, rigidityResErr);

  }
}

void RigidityResolutionPlot::finalize()
{
  update();
  /*if (histogram(0)->GetEntries() > 3)
    histogram(0)->Fit(function(1),"q0");

  TF1* fittedRes = new TF1(*expectedRes);

  expectedRes->SetLineColor(kBlack);
  expectedRes->SetLineStyle(kDashed);
  fittedRes->SetLineColor(kRed);
  addFunction(expectedRes);
  addFunction(fittedRes);
  */


  //saveHistos();
}

void RigidityResolutionPlot::saveHistos()
{
  for (int bin = 1; bin <= m_numberOfBins; ++bin) {
    TH1D* hist = m_trackerResolutionHistograms[bin-1];
    if (hist->GetEntries() < 10)
      continue;
    double genRig = histogram(0)->GetBinCenter(bin);
    hist->SaveAs(qPrintable(QString("rigres_for_%1_at_%2GV.root").arg(m_particleProperties.name()).arg(genRig)));
  }
}

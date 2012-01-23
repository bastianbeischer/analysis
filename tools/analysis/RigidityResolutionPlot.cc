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
#include "Hypothesis.hh"

#include <TH1D.h>
#include <TF1.h>
#include <TLatex.h>
#include <TMath.h>
#include <TLegend.h>
#include <TLatex.h>

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
  , m_trackerFunction(0)
  , m_tofFunction(0)
  , m_trackerResolutionGauss(new TF1("trackerResolutionGauss", this, &RigidityResolutionPlot::trackerGauss, -1., 1, 3, "RigidityResolutionPlot", "trackerGauss"))
  , m_tofResolutionGauss(new TF1("tofResolutionGauss", this, &RigidityResolutionPlot::tofGauss, -1., 1, 5, "RigidityResolutionPlot", "tofGauss"))
  , m_trackerResolutionHistograms(m_numberOfBins, 0)
  , m_tofResolutionHistograms(m_numberOfBins, 0)
  , m_weightedMeanResolutionHistograms(m_numberOfBins, 0)
  , m_chi2ResolutionHistograms(m_numberOfBins, 0)
  , m_likelihoodResolutionHistograms(m_numberOfBins, 0)
  , m_projectionTrackerLatex(RootPlot::newLatex(0.65, 0.85))
  , m_projectionTofLatex(newLatex(0.65, 0.85))
  , m_projectionWeightedMeanLatex(newLatex(0.65, 0.85))
  , m_projectionChi2FitLatex(newLatex(0.65, 0.85))
  , m_projectionLikelihoodLatex(newLatex(0.65, 0.65))
{
  QString particleName = m_particleProperties.name();
  QString title = "rigidity resolution " + particleName;

  TH1D* h = 0;

  TLegend* legend = new TLegend(.72, .70, .83, .88);
  addLegend(legend);

  h = new TH1D(qPrintable("tracker " + title), "", m_numberOfBins, m_min, m_max);
  h->SetMarkerColor(kBlue);
  h->SetLineColor(kBlue);
  legend->AddEntry(h, "tracker", "L");
  addHistogram(h);

  h = new TH1D(qPrintable("TOF " + title), "", m_numberOfBins, m_min, m_max);
  h->SetMarkerColor(kMagenta);
  h->SetLineColor(kMagenta);
  legend->AddEntry(h, "TOF", "L");
  addHistogram(h);

  h = new TH1D(qPrintable("weighted mean " + title), "", m_numberOfBins, m_min, m_max);
  h->SetMarkerColor(kGreen);
  h->SetLineColor(kGreen);
  legend->AddEntry(h, "weighted mean", "L");
  addHistogram(h);

  h = new TH1D(qPrintable("chi2 fit " + title), "", m_numberOfBins, m_min, m_max);
  h->SetMarkerColor(kRed);
  h->SetLineColor(kRed);
  legend->AddEntry(h, "chi2 fit", "L");
  addHistogram(h);

  addHistogram(new TH1D(qPrintable("likelihood " + title), "", m_numberOfBins, m_min, m_max));

  m_trackerFunction = newTrackerResolutionFunction("expected tracker resolution " + particleName);
  addFunction(m_trackerFunction);

  m_tofFunction = newTofResolutionFunction("expected TOF resolution " + particleName);
  addFunction(m_tofFunction);

  addFunction(newWeightedMeanResolutionFunction("expected weighted mean resolution " + particleName));

  for (int bin = 1; bin <= m_numberOfBins; ++bin) {
    KineticVariable generated(m_particleProperties.type(), Enums::AbsoluteRigidity, xAxis()->GetBinCenter(bin));
    QString histTitle = QString("rigidity resolution for %1 at %2 GV").arg(particleName).arg(generated.absoluteRigidity());
    int resolutionBins = 100;
    double curvatureRange = 5 * m_trackerFunction->Eval(generated.absoluteRigidity());
    m_trackerResolutionHistograms[bin-1] = new TH1D(qPrintable("tracker " + histTitle), "spectrometer;1/R / 1/GV;",
      resolutionBins, generated.curvature() - curvatureRange, generated.curvature() + curvatureRange);
    m_tofResolutionHistograms[bin-1] = new TH1D(qPrintable("TOF " + histTitle), "TOF;1/#beta;",
      resolutionBins, 0., 10.);
    m_weightedMeanResolutionHistograms[bin-1] = new TH1D(qPrintable("weighted mean " + histTitle), "weighted mean;1/R / 1/GV;",
      resolutionBins, generated.curvature() - curvatureRange, generated.curvature() + curvatureRange);
    m_chi2ResolutionHistograms[bin-1] = new TH1D(qPrintable("chi2 fit " + histTitle), "chi2 fit;1/R / 1/GV;",
      resolutionBins, generated.curvature() - curvatureRange, generated.curvature() + curvatureRange);
    m_likelihoodResolutionHistograms[bin-1] = new TH1D(qPrintable("likelihood " + histTitle), "likelihood;1/R / 1/GV;",
      resolutionBins, generated.curvature() - curvatureRange, generated.curvature() + curvatureRange);
  }

  TLatex* ltx = 0;
  ltx = newLatex(0.6, 0.35);
  ltx->SetTitle("#sigma_{R}/R = aR/GV  #oplus b/#beta");
  ltx->SetTextSizePixels(40);
  addLatex(ltx);
  ltx = newLatex(0.6, 0.27);
  ltx->SetTitle(qPrintable(QString("%1: a = %2, b = %3").arg(m_particleProperties.name())
    .arg(m_trackerFunction->GetParameter("a"), 0, 'f', 3).arg(m_trackerFunction->GetParameter("b"), 0, 'f', 3)));
  ltx->SetTextSizePixels(40);
  addLatex(ltx);
  ltx = newLatex(0.6, 0.19);
  ltx->SetTitle(qPrintable(QString("#sigma_{TOF} = %1 ns").arg(m_tofFunction->GetParameter("sigma"), 0, 'f', 2)));
  ltx->SetTextSizePixels(40);
  addLatex(ltx);

  setTitle(title);
  setAxisTitle("R / GV", "#sigma_{R}/R");
  yAxis()->SetRangeUser(0, 1.5);

  m_rigDistributionWidget->GetCanvas()->Divide(5, 1);
  setSecondaryWidget(m_rigDistributionWidget);

}

RigidityResolutionPlot::~RigidityResolutionPlot()
{
  qDeleteAll(m_trackerResolutionHistograms);
  delete m_projectionTrackerLatex;
  delete m_projectionTofLatex;
  delete m_projectionWeightedMeanLatex;
  delete m_projectionChi2FitLatex;
  delete m_projectionLikelihoodLatex;
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
    default: f->SetParameters(0.077, 0.3175); break;
  }
  f->SetLineColor(kBlue);
  return f;
}

TF1* RigidityResolutionPlot::newTofResolutionFunction(const QString& name) const
{
  TF1* f = new TF1(qPrintable(name), this, &RigidityResolutionPlot::tofResolution,
    m_min, m_max, 1, "RigidityResolutionPlot", "tofResolution");
  f->SetParNames("sigma");
  f->SetParameter(0, 0.40);
  f->SetLineColor(kMagenta);
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
  f->SetLineColor(kGreen);
  return f;
}

double RigidityResolutionPlot::trackerGauss(double* x, double* p) const
{
  return p[0] * TMath::Gaus(x[0], p[1], p[2]);
}

double RigidityResolutionPlot::tofGauss(double* x, double* p) const
{
  // Unfortunately, the testbeam samples are contaminated with light particles.
  // I.e. there is always a double peak of protons/antiprotons (beta < 1 for p < 1)
  // and pion/muons/electrons (beta = 1 for all p).
  return p[0] * TMath::Gaus(x[0], p[1], p[2]) + p[3] * TMath::Gaus(x[0], 1., p[2]);
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
  if (!m_rigDistributionWidget->isVisible())
    return;
  TVirtualPad* prevPad = gPad;
  TCanvas* canvas = m_rigDistributionWidget->GetCanvas();
  canvas->cd(1)->Clear();
  canvas->cd(2)->Clear();
  canvas->cd(3)->Clear();
  canvas->cd(4)->Clear();
  canvas->cd(5)->Clear();
  int bin = histogram(0)->FindBin(posX);
  if (bin < 1 || m_numberOfBins < bin) {
    prevPad->cd();
    return;
  }

  KineticVariable variable(m_particleProperties.type(), Enums::AbsoluteRigidity, xAxis()->GetBinCenter(bin));

  canvas->cd(1);
  m_trackerResolutionHistograms[bin-1]->Draw();
  m_projectionTrackerLatex->SetText(0.65, 0.85, qPrintable(QString("n = %1").arg(m_trackerResolutionHistograms[bin-1]->GetEntries())));
  m_projectionTrackerLatex->Draw();
  if (fitTracker(m_trackerResolutionHistograms[bin-1], variable.curvature()))
    m_trackerResolutionGauss->Draw("SAME");

  canvas->cd(2);
  m_tofResolutionHistograms[bin-1]->Draw();
  m_projectionTofLatex->SetText(0.65, 0.85, qPrintable(QString("n = %1").arg(m_tofResolutionHistograms[bin-1]->GetEntries())));
  m_projectionTofLatex->Draw();
  if (fitTof(m_tofResolutionHistograms[bin-1], variable.inverseBeta()))
    m_tofResolutionGauss->Draw("SAME");

  canvas->cd(3);
  m_projectionWeightedMeanLatex->SetText(0.65, 0.85, qPrintable(QString("n = %1").arg(m_weightedMeanResolutionHistograms[bin-1]->GetEntries())));
  m_projectionWeightedMeanLatex->Draw();
  m_weightedMeanResolutionHistograms[bin-1]->Draw();

  canvas->cd(4);
  m_projectionChi2FitLatex->SetText(0.65, 0.85, qPrintable(QString("n = %1").arg(m_chi2ResolutionHistograms[bin-1]->GetEntries())));
  m_projectionChi2FitLatex->Draw();
  m_chi2ResolutionHistograms[bin-1]->Draw();

  canvas->cd(5);
  m_projectionLikelihoodLatex->SetText(0.65, 0.85, qPrintable(QString("n = %1").arg(m_likelihoodResolutionHistograms[bin-1]->GetEntries())));
  m_projectionLikelihoodLatex->Draw();
  m_likelihoodResolutionHistograms[bin-1]->Draw();

  canvas->cd();
  canvas->Modified();
  canvas->Update();
  prevPad->cd();
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

  const QMap<Enums::ReconstructionMethod, Hypothesis*>& hypotheses = event->particle()->hypotheses();
  QMap<Enums::ReconstructionMethod, Hypothesis*>::ConstIterator it = hypotheses.constBegin();
  const QMap<Enums::ReconstructionMethod, Hypothesis*>::ConstIterator end = hypotheses.constEnd();

  int bin = histogram(0)->FindBin(qAbs(referenceRigidity(event)));
  Q_ASSERT(0 < bin  && bin <= m_numberOfBins);
  while (it != end) {
    const Hypothesis* h = it.value();
    if (it.key() == Enums::SpectrometerExternalInformation && h->particle() == m_particleProperties.type()) {
      m_trackerResolutionHistograms[bin-1]->Fill(h->curvature());
    } else if (it.key() == Enums::TOFExternalInformation && h->particle() == m_particleProperties.type()) {
      m_tofResolutionHistograms[bin-1]->Fill(h->inverseBeta());
    } else if (it.key() == Enums::WeightedMeanExternalInformation && h->particle() == m_particleProperties.type()) {
      m_weightedMeanResolutionHistograms[bin-1]->Fill(h->curvature());
    } else if (it.key() == Enums::Chi2ExternalInformation && h->particle() == m_particleProperties.type()) {
      m_chi2ResolutionHistograms[bin-1]->Fill(h->curvature());
    } else if (it.key() == Enums::LikelihoodExternalInformation && h->particle() == m_particleProperties.type()) {
      m_likelihoodResolutionHistograms[bin-1]->Fill(h->curvature());
    }
    ++it;
  }
}

bool RigidityResolutionPlot::fitTracker(TH1D* h, double curvature)
{
  if (h->GetEntries() < 10)
    return false;
  double mean = h->GetMean();
  double rms = h->GetRMS();
  m_trackerResolutionGauss->SetParameter(0, h->GetMaximum());
  m_trackerResolutionGauss->FixParameter(1, curvature);
  m_trackerResolutionGauss->SetParameter(2, rms);
  m_trackerResolutionGauss->SetRange(mean - 2*rms, mean + 2*rms);
  h->Fit(m_trackerResolutionGauss, "RQN0");
  return true;
}

bool RigidityResolutionPlot::fitTof(TH1D* h, double inverseBeta)
{
  if (h->GetEntries() < 10)
    return false;
  m_tofResolutionGauss->SetParameter(0, h->GetBinContent(h->FindBin(inverseBeta)));
  m_tofResolutionGauss->FixParameter(1, inverseBeta);
  m_tofResolutionGauss->SetParameter(2, 0.4);
  m_tofResolutionGauss->SetParameter(3, h->GetBinContent(h->FindBin(1.)));
  m_tofResolutionGauss->SetParameter(4, 0.4);
  m_tofResolutionGauss->SetRange(1., h->GetXaxis()->GetXmax());
  h->Fit(m_tofResolutionGauss, "RQN0");
  return true;
}

void RigidityResolutionPlot::update()
{
  histogram(0)->Reset();
  histogram(1)->Reset();
  histogram(2)->Reset();
  histogram(3)->Reset();
  histogram(4)->Reset();
  for (int bin = 1; bin <= m_numberOfBins; ++bin) {
    KineticVariable generated(m_particleProperties.type(), Enums::AbsoluteRigidity, xAxis()->GetBinCenter(bin));
    if (fitTracker(m_trackerResolutionHistograms[bin-1], generated.curvature())) {
      double sigmaCurvature = m_trackerResolutionGauss->GetParameter(2);
      double sigmaCurvatureError = m_trackerResolutionGauss->GetParError(2);
      histogram(0)->SetBinContent(bin, sigmaCurvature / qAbs(generated.curvature()));
      histogram(0)->SetBinError(bin, sigmaCurvatureError / qAbs(generated.curvature()));
    }
    if (fitTof(m_tofResolutionHistograms[bin-1], generated.inverseBeta())) {
      double beta = generated.beta();
      double sigmaInverseBeta = m_tofResolutionGauss->GetParameter(2);
      double sigmaKOverK = sigmaInverseBeta * beta / (1 - beta*beta);
      histogram(1)->SetBinContent(bin, sigmaInverseBeta * beta / (1 - beta*beta));
      double sigmaSigmaInverseBetaError = m_tofResolutionGauss->GetParError(2);
      double sigmaSigmaKOverK = sigmaKOverK * sigmaSigmaInverseBetaError / sigmaInverseBeta;
      histogram(1)->SetBinError(bin, sigmaSigmaKOverK);
    }
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
}

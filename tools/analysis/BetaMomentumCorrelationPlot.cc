#include "BetaMomentumCorrelationPlot.hh"
#include "BrokenLine.hh"
#include "Constants.hh"

#include "ParticleInformation.hh"
#include "Hit.hh"
#include "TOFCluster.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Hypothesis.hh"
#include "TimeOfFlight.hh"
#include "Track.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>

#include <QDebug>

double betaMomentumCorrelationFunction(double x[], double p[])
{
  double rigidity = qAbs(x[0]) < 0.01 ? 0.01 : qAbs(x[0]);
  double momentum = rigidity * qAbs(p[1]);
  return sqrt(momentum*momentum+p[0]*p[0])/momentum;
}

TF1* BetaMomentumCorrelationPlot::newParticleFunction(Enums::Particle particle, double range)
{
  ParticleProperties properties(particle);
  bool negative = properties.charge() < 0;
  TF1* f = new TF1(qPrintable(properties.name() + " beta momentum correlation function"),
    betaMomentumCorrelationFunction, negative ? -range : 0.01, negative ? -0.01 : range, 2);
  f->SetParameters(properties.mass(), properties.charge());
  f->SetNpx(1000);
  f->SetLineColor(properties.color());
  f->SetLineStyle(kDashed);
  return f;
}

BetaMomentumCorrelationPlot::BetaMomentumCorrelationPlot(bool hypothesisRigidity, bool hypothesisBeta)
  : AnalysisPlot(Enums::MomentumReconstruction)
  , H2DPlot()
  , m_hypothesisRigidity(hypothesisRigidity)
  , m_hypothesisBeta(hypothesisBeta)
{
  QString title = "beta ";
  if (m_hypothesisBeta) title+= "from hypothesis ";
  title+= "vs momentum";
  if (m_hypothesisRigidity) title+= " from hypothesis";
  setTitle(title);
  int nBinsX = 121;
  double range = 6;
  int nBinsY = 201;
  double yMin = -10;
  double yMax = 10;
  double binWidthY = (yMax - yMin) / nBinsY;
  TH2D* histogram = new TH2D(qPrintable(title), "", nBinsX, -range, +range,
    nBinsY, yMin - binWidthY / 2., yMax + binWidthY / 2.);
  addHistogram(histogram);

  setAxisTitle(hypothesisRigidity ? "R_{hypothesis} / GV" : "R / GV", hypothesisBeta ? "1/#beta_{hypothesis}" : "1/#beta", "");

  QVector<Enums::Particle> functionParticles = QVector<Enums::Particle>() << Enums::Proton << Enums::Helium
    << Enums::Electron << Enums::Positron << Enums::PiPlus << Enums::PiMinus << Enums::Muon << Enums::AntiMuon;
  TLegend* legend = new TLegend(.12, .72, .23, .88);
  legend->SetMargin(.7);
  foreach (Enums::Particle particle, functionParticles) {
    TF1* f = newParticleFunction(particle, range);
    addFunction(f);
    legend->AddEntry(f, qPrintable(Enums::label(particle)), "l");
  }
  addLegend(legend);

  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good | Enums::InsideMagnet | Enums::BetaGood);
}

BetaMomentumCorrelationPlot::~BetaMomentumCorrelationPlot()
{
}

void BetaMomentumCorrelationPlot::processEvent(const AnalyzedEvent* event)
{
  const Hypothesis* h = event->particle()->hypothesis();
  double rigidity = m_hypothesisRigidity ? h->rigidity() : event->particle()->track()->rigidity();
  double beta = m_hypothesisBeta ? h->beta() : event->particle()->timeOfFlight()->beta();
  if (!qIsNull(beta))
    histogram()->Fill(rigidity, 1./beta);
}

void BetaMomentumCorrelationPlot::finalize()
{
  for (int i = 0; i < numberOfHistograms(); ++i) {
    TH2D* h = histogram(i);
    int entries = 0;
    for (int binX = 1; binX <= h->GetXaxis()->GetNbins(); ++binX)
      for (int binY = 1; binY <= h->GetYaxis()->GetNbins(); ++binY)
        entries+= h->GetBinContent(binX, binY);
    if (0 < entries) {
      for (int binX = 1; binX <= h->GetXaxis()->GetNbins(); ++binX)
        for (int binY = 1; binY <= h->GetYaxis()->GetNbins(); ++binY)
          h->SetBinContent(binX, binY, h->GetBinContent(binX, binY) / entries);
      h->SetEntries(100);
    }
  }
}

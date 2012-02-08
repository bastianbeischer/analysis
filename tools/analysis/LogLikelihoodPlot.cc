#include "LogLikelihoodPlot.hh"
#include "Particle.hh"
#include "Hypothesis.hh"
#include "ParticleProperties.hh"
#include "SensorTypes.hh"
#include "SimpleEvent.hh"
#include "Helpers.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TLine.h>
#include <TCanvas.h>

#include <QDebug>

#include <cmath>

LogLikelihoodPlot::LogLikelihoodPlot(Enums::Particle signalParticle, Enums::Particles backgroundParticles)
  : AnalysisPlot(Enums::LikelihoodTopic)
  , H1DPlot()
  , m_signalParticle(signalParticle)
  , m_backgroundParticles(backgroundParticles)
  , m_histogram(0)
  , m_line(new TLine(2*log(2), 0, 2*log(2), 1))
{
  Q_ASSERT((signalParticle & backgroundParticles) == 0);

  setTitle("ln L distribution " + Enums::label(signalParticle));

  QVector<double> binning = Helpers::logLikelihoodBinning();
  addHistogram(new TH1D(qPrintable(title()), "", binning.count() - 1, binning.constData()));
  histogram()->SetLineColor(ParticleProperties(signalParticle).color());

  QString sLabel = Helpers::greekifyLetters(Enums::label(m_signalParticle));
  QString bgLabel = Helpers::greekifyLetters(Enums::label(m_backgroundParticles));
  setAxisTitle("-2 ln (L_{" + sLabel + "} / (L_{" + sLabel + "} + L_{"+ bgLabel + "}))", "");

  addLatex(RootPlot::newLatex(.65, .85));
  addLatex(RootPlot::newLatex(.65, .82));
  addLatex(RootPlot::newLatex(.65, .79));
  addLatex(RootPlot::newLatex(.65, .76));

  double x = -2. * log(1./2.);

  m_line = new TLine(x, 0, x, 1);
  m_line->SetLineWidth(2);
  m_line->SetLineColor(kRed);
  m_line->SetLineStyle(kDashed);

  TLatex* latex = 0;
  latex = RootPlot::newLatex(.16, .85);
  latex->SetTitle(qPrintable("L_{" + sLabel + "} = L_{" + bgLabel + "}"));
  addLatex(latex);
  latex = RootPlot::newLatex(.12, .80);
  latex->SetTitle(qPrintable(sLabel));
  addLatex(latex);
  latex = RootPlot::newLatex(.16, .80);
  latex->SetTitle(qPrintable(bgLabel));
  addLatex(latex);
}

void LogLikelihoodPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::AllTrackerLayers | ParticleInformation::InsideMagnet | ParticleInformation::Chi2Good | ParticleInformation::BetaGood))
    return;
  const Particle* particle = event->particle();
  if (particle->hypothesis()->rigidity() < 0.2)
    return;
  double signal = particle->signalLikelihood(m_signalParticle);
  double background = particle->backgroundLikelihood(m_backgroundParticles);
  double logL = -2 * log (signal / (signal + background));
  histogram()->Fill(logL);
}

void LogLikelihoodPlot::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  double max = 1.15 * histogram()->GetMaximum();
  m_line->SetY2(max);
  m_line->Draw();
}

void LogLikelihoodPlot::update()
{
  TH1* h = histogram();
  double max = 1.05 * h->GetMaximum();
  m_line->SetY2(max);
  latex(0)->SetTitle(qPrintable(QString("n     = %1").arg(histogram()->GetEntries())));
  latex(1)->SetTitle(qPrintable(QString("uflow = %1").arg(histogram()->GetBinContent(0))));
  latex(2)->SetTitle(qPrintable(QString("oflow = %1").arg(histogram()->GetBinContent(histogram()->GetXaxis()->GetNbins()+1))));

  double x = -2 * log (1./2.);
  int signal = 0;
  int background = 0;
  for (int bin = 1; bin <= h->GetXaxis()->GetNbins(); ++bin) {
    if (h->GetXaxis()->GetBinUpEdge(bin) < x) {
      signal+= h->GetBinContent(bin);
    } else {
      background+= h->GetBinContent(bin);
    }
  }
  latex(3)->SetTitle(qPrintable(QString("ratio = %1\%").arg(100. * signal / (signal + background))));
}

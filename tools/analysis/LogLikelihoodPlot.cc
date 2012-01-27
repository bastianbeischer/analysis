#include "LogLikelihoodPlot.hh"
#include "Particle.hh"
#include "Hypothesis.hh"
#include "ParticleProperties.hh"
#include "SensorTypes.hh"
#include "SimpleEvent.hh"

#include <TH1D.h>
#include <TLatex.h>

#include <QDebug>

#include <cmath>

LogLikelihoodPlot::LogLikelihoodPlot(Enums::Particle signalParticle, Enums::Particles backgroundParticles)
  : AnalysisPlot(Enums::LikelihoodTopic)
  , H1DPlot()
  , m_signalParticle(signalParticle)
  , m_backgroundParticles(backgroundParticles)
  , m_histogram(0)
{
  Q_ASSERT((signalParticle & backgroundParticles) == 0);

  setTitle("ln L distribution " + Enums::label(signalParticle));

  addHistogram(new TH1D(qPrintable(title()), "", 2000, -10., 190.));
  histogram()->SetLineColor(ParticleProperties(signalParticle).color());

  QString sLabel = Enums::label(m_signalParticle);
  QString bgLabel = Enums::label(m_backgroundParticles);
  QString axisTitle = "-2 ln (L_{" + sLabel + "} / (L_{" + sLabel + "} + L_{"+ bgLabel + "})";
  axisTitle.replace("alpha", "#alpha");
  axisTitle.replace("mu", "#mu");
  axisTitle.replace("pi", "#pi");
  setAxisTitle(axisTitle, "");

  addLatex(RootPlot::newLatex(.15, .85));
  latex(0)->SetTextColor(kRed);
  addLatex(RootPlot::newLatex(.15, .82));
  latex(1)->SetTextColor(kRed);
  addLatex(RootPlot::newLatex(.15, .79));
  latex(2)->SetTextColor(kRed);
}

void LogLikelihoodPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::AllTrackerLayers | ParticleInformation::InsideMagnet | ParticleInformation::Chi2Good | ParticleInformation::BetaGood))
    return;
  const Particle* particle = event->particle();
  double signal = particle->signalLikelihood(m_signalParticle);
  double background = particle->backgroundLikelihood(m_backgroundParticles);
  double logL = -2 * log (signal / (signal + background));
  histogram()->Fill(logL);
}

void LogLikelihoodPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("n     = %1").arg(histogram()->GetEntries())));
  latex(1)->SetTitle(qPrintable(QString("uflow = %1").arg(histogram()->GetBinContent(0))));
  latex(2)->SetTitle(qPrintable(QString("oflow = %1").arg(histogram()->GetBinContent(histogram()->GetXaxis()->GetNbins()+1))));
}

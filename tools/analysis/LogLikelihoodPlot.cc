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
  , m_nBins(200)
  , m_normalized(false)
  , m_signalHypothesisHistogram(0)
  , m_backgroundHypothesisHistogram(0)
  , m_typeToIndex()
  , m_backgroundHypothesisHistograms()
{
  Q_ASSERT((signalParticle & backgroundParticles) == 0);

  setTitle("ln L distribution " + Enums::label(signalParticle));

  double min = -10.;
  double max = +100.;

  TH1D* h = 0;

  m_signalHypothesisHistogram = new TH1D(qPrintable(title() + " signal"), "", m_nBins, min, max);
  h = new TH1D(qPrintable(title() + " signal normalized"), "", m_nBins, min, max);
  h->SetLineColor(ParticleProperties(signalParticle).color());
  addHistogram(h);

  h = new TH1D(qPrintable(title() + " signal normalized"), "", m_nBins, min, max);
  h->SetLineColor(kBlack);
  addHistogram(h);

  m_backgroundHypothesisHistogram = new TH1D(qPrintable(title() + " background"), "", m_nBins, min, max);

  Enums::ParticleIterator end = Enums::particleEnd();
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != end; ++it)
    if (it.key() != Enums::NoParticle && (it.key() & backgroundParticles)) {
      m_typeToIndex.insert(it.key(), m_backgroundHypothesisHistograms.count());
      h = new TH1D(qPrintable(title() + " background " + it.value()), "", m_nBins, min, max);
      m_backgroundHypothesisHistograms.append(h);
      h = new TH1D(qPrintable(title() + " background normalized " + it.value()), "", m_nBins, min, max);
      h->SetLineColor(ParticleProperties(it.key()).color());
      addHistogram(h);
    }

  addHistogram(new TH1D(qPrintable(title() + " background normalized"), "", m_nBins, min, max));

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
  addLatex(RootPlot::newLatex(.15, .75));
  latex(3)->SetTextColor(kBlue);
  addLatex(RootPlot::newLatex(.15, .72));
  latex(4)->SetTextColor(kBlue);
  addLatex(RootPlot::newLatex(.15, .69));
  latex(5)->SetTextColor(kBlue);
}

void LogLikelihoodPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::AllTrackerLayers | ParticleInformation::InsideMagnet | ParticleInformation::Chi2Good | ParticleInformation::BetaGood))
    return;

  const Particle* particle = event->particle();
  const Hypothesis* hypothesis = particle->hypothesis();
  double signal = particle->signalLikelihood(m_signalParticle);
  double background = particle->backgroundLikelihood(m_backgroundParticles);
  double logL = -2 * log (signal / (signal + background));

  if (hypothesis->particle() == m_signalParticle) {
    m_signalHypothesisHistogram->Fill(logL);
  } else {
    m_backgroundHypothesisHistogram->Fill(logL);

    QMap<Enums::Particle, int>::ConstIterator it = m_typeToIndex.find(hypothesis->particle());
    Q_ASSERT(it != m_typeToIndex.end());
    m_backgroundHypothesisHistograms[it.value()]->Fill(logL);
  }
}

void LogLikelihoodPlot::update()
{
  int n = 0;

  n = m_normalized ? m_signalHypothesisHistogram->GetEntries() : 1;
  for (int bin = 1; bin <= m_nBins; ++bin)
    histogram(0)->SetBinContent(bin, m_signalHypothesisHistogram->GetBinContent(bin) / n);
  histogram(0)->SetEntries(n);

  n = m_normalized ? m_backgroundHypothesisHistogram->GetEntries() : 1;
  for (int bin = 1; bin <= m_nBins; ++bin)
    histogram(1)->SetBinContent(bin, m_backgroundHypothesisHistogram->GetBinContent(bin) / n);
  histogram(1)->SetEntries(n);

  for (int i = 0; i < m_backgroundHypothesisHistograms.count(); ++i)
    for (int bin = 1; bin <= m_nBins; ++bin)
      histogram(i+2)->SetBinContent(bin, m_backgroundHypothesisHistograms[i]->GetBinContent(bin) / n);

  latex(0)->SetTitle(qPrintable(QString("n     = %1").arg(m_signalHypothesisHistogram->GetEntries())));
  latex(1)->SetTitle(qPrintable(QString("uflow = %1").arg(m_signalHypothesisHistogram->GetBinContent(0))));
  latex(2)->SetTitle(qPrintable(QString("oflow = %1").arg(m_signalHypothesisHistogram->GetBinContent(m_nBins+1))));

  latex(3)->SetTitle(qPrintable(QString("n     = %1").arg(m_backgroundHypothesisHistogram->GetEntries())));
  latex(4)->SetTitle(qPrintable(QString("uflow = %1").arg(m_backgroundHypothesisHistogram->GetBinContent(0))));
  latex(5)->SetTitle(qPrintable(QString("oflow = %1").arg(m_backgroundHypothesisHistogram->GetBinContent(m_nBins+1))));
}

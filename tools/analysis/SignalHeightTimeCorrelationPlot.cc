#include "SignalHeightTimeCorrelationPlot.hh"

#include "Track.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "SimpleEvent.hh"

#include <QString>
#include <QDebug>

#include <TH2D.h>
#include <TH1D.h>

SignalHeightTimeCorrelationPlot::SignalHeightTimeCorrelationPlot(unsigned short sipmId, const QDateTime& first, const QDateTime& last)
  : AnalysisPlot(Enums::SignalHeightTracker)
  , H2DProjectionPlot()
  , m_sipmId(sipmId)
  , m_histo(0)
  , m_normHisto(0)
{
  setTitle(QString("signal height time correlcation 0x%1").arg(m_sipmId, 0, 16));
  int t1 = first.toTime_t();
  t1-= (t1 % 60) + 60;
  int t2 = last.toTime_t();
  t2+= 120 - (t2 % 60);
  const unsigned int nTimeBins = 10;
  const unsigned int nSignalHeightBins = 70;
  const double minSignalHeight = 0;
  const double maxSignalHeight = 7000;
  TH2D* histo = new TH2D(qPrintable(title()), "", nTimeBins, t1, t2, nSignalHeightBins, minSignalHeight, maxSignalHeight);
  histo->GetXaxis()->SetTimeDisplay(1);
  histo->GetXaxis()->SetTimeFormat("%d-%H:%M");
  setAxisTitle("time", "signal height / adc counts", "");
  m_histo = new TH2D(*histo);
  addHistogram(histo);
  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", nTimeBins, t1, t2);
}

SignalHeightTimeCorrelationPlot::~SignalHeightTimeCorrelationPlot()
{
  delete m_histo;
  delete m_normHisto;
}

void SignalHeightTimeCorrelationPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
{
  const Track* track = particle->track();
  if (!track)
    return;
  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::Chi2Good))
    return;
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;
//  if (flags & ParticleInformation::MagnetCollision)
//    return;
  const QVector<Hit*>::const_iterator endIt = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tracker && hit->elementId() == m_sipmId) {
      const double eventTime = event->time();
      m_histo->Fill(eventTime, hit->signalHeight());
      m_normHisto->Fill(eventTime);
    }
  }
}

void SignalHeightTimeCorrelationPlot::update()
{
  TH2D* hist = m_histo;
  for (int xBin = 1; xBin <= hist->GetNbinsX(); xBin++)
    for (int yBin = 1; yBin <= hist->GetNbinsY(); yBin++)
      histogram()->SetBinContent(xBin, yBin, hist->GetBinContent(xBin, yBin));
}

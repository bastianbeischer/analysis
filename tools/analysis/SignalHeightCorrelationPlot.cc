#include "SignalHeightCorrelationPlot.hh"

#include "Track.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "SimpleEvent.hh"
#include "Setup.hh"
#include "Hypothesis.hh"

#include <QString>
#include <QDebug>
#include <QDateTime>

#include <cmath>

#include <TH2D.h>
#include <TH1D.h>

SignalHeightCorrelationPlot::SignalHeightCorrelationPlot(unsigned short id, CorrelationType type, TH2D* histogram)
  : AnalysisPlot(Enums::SignalHeightTracker)
  , H2DProjectionPlot()
  , m_id(id)
  , m_type(type)
  , m_histogram(histogram)
  , m_normHisto(0)
{
  setup();
}

SignalHeightCorrelationPlot::SignalHeightCorrelationPlot(CorrelationType type, TH2D* histogram)
  : AnalysisPlot(Enums::SignalHeightTracker)
  , H2DProjectionPlot()
  , m_id(0)
  , m_type(type)
  , m_histogram(histogram)
  , m_normHisto(0)
{
  setup();
}

void SignalHeightCorrelationPlot::setup()
{
  setTitle(m_histogram->GetName());
  setAxisTitle(m_histogram->GetXaxis()->GetTitle(), m_histogram->GetYaxis()->GetTitle(), "");
  addHistogram(new TH2D(*m_histogram));
  int nBins = m_histogram->GetNbinsX();
  QVector<double> binning(nBins + 1);
  for (int bin = 0; bin <= nBins; ++bin)
    binning[bin] = m_histogram->GetXaxis()->GetBinLowEdge(bin + 1);
  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", nBins, binning.constData());

  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good | Enums::AllTrackerLayers);
  if (m_type == Rigidity)
    addRequiredEventFlags(Enums::InsideMagnet);
}

SignalHeightCorrelationPlot::~SignalHeightCorrelationPlot()
{
  delete m_histogram;
  delete m_normHisto;
}

void SignalHeightCorrelationPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  int nHitsOnTrack = 0;
  double sumSignalHeightOnTrack = 0;
  const QVector<Hit*>::const_iterator endIt = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (m_type == Rigidity) {
      if (hit->type() == Hit::tracker) {
        sumSignalHeightOnTrack += hit->signalHeight();
        ++nHitsOnTrack;
      }
    } else {
      if (hit->type() == Hit::tracker && hit->elementId() == m_id) {
        double value = 0.;
        if (m_type == Temperature)
          value = event->simpleEvent()->sensorData(Setup::instance()->sensorForId(hit->elementId()));
        else if (m_type == Time)
          value = event->simpleEvent()->time();
        else if (m_type == Undefined)
          Q_ASSERT(false);
        if (std::isnan(value))
          return;
        m_histogram->Fill(value, hit->signalHeight());
        m_normHisto->Fill(value);
      }
    }
  }
  if (m_type == Rigidity)
    m_histogram->Fill(event->particle()->hypothesis()->rigidity(), sumSignalHeightOnTrack / nHitsOnTrack);
}

void SignalHeightCorrelationPlot::update()
{
  for (int xBin = 1; xBin <= m_histogram->GetNbinsX(); xBin++)
    for (int yBin = 1; yBin <= m_histogram->GetNbinsY(); yBin++)
      histogram()->SetBinContent(xBin, yBin, m_histogram->GetBinContent(xBin, yBin));
}

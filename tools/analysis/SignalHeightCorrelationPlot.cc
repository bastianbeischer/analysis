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
  , m_histo(histogram)
  , m_normHisto(0)
{
  setTitle(histogram->GetName());
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle(), "");
  addHistogram(new TH2D(*histogram));
  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", histogram->GetNbinsX(), histogram->GetXaxis()->GetXbins()->GetArray());
}

SignalHeightCorrelationPlot::SignalHeightCorrelationPlot(CorrelationType type, TH2D* histogram)
  : AnalysisPlot(Enums::SignalHeightTracker)
  , H2DProjectionPlot()
  , m_id(0)
  , m_type(type)
  , m_histo(histogram)
  , m_normHisto(0)
{
  setTitle(histogram->GetName());
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle(), "");
  addHistogram(new TH2D(*histogram));
  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", histogram->GetNbinsX(), histogram->GetXaxis()->GetXbins()->GetArray());
}

SignalHeightCorrelationPlot::~SignalHeightCorrelationPlot()
{
  delete m_histo;
  delete m_normHisto;
}

void SignalHeightCorrelationPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  if (!track)
    return;
  ParticleInformation::Flags flags = event->particle()->information()->flags();
  if (!(flags & ParticleInformation::Chi2Good))
    return;
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;
  if (m_type == Rigidity && !(flags & ParticleInformation::InsideMagnet))
    return;
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
        m_histo->Fill(value, hit->signalHeight());
        m_normHisto->Fill(value);
      }
    }
  }
  if (m_type == Rigidity)
    m_histo->Fill(event->particle()->hypothesis()->rigidity(), sumSignalHeightOnTrack / nHitsOnTrack);
}

void SignalHeightCorrelationPlot::update()
{
  for (int xBin = 1; xBin <= m_histo->GetNbinsX(); xBin++)
    for (int yBin = 1; yBin <= m_histo->GetNbinsY(); yBin++)
      histogram()->SetBinContent(xBin, yBin, m_histo->GetBinContent(xBin, yBin));
}

#include "SignalHeightTemperatureCorrelationPlot.hh"

#include "Track.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "SimpleEvent.hh"
#include "Setup.hh"

#include <QString>
#include <QDebug>

#include <cmath>

#include <TH2D.h>
#include <TH1D.h>

SignalHeightTemperatureCorrelationPlot::SignalHeightTemperatureCorrelationPlot(unsigned short sipmId)
: AnalysisPlot(Enums::SignalHeightTracker)
, H2DProjectionPlot()
, m_sipmId(sipmId)
, m_histo(0)
, m_normHisto(0)
{
  setTitle(QString("signal height temperature correlcation 0x%1").arg(m_sipmId, 0, 16));
  const unsigned int nTemperatureBins = 10;
  const double minTemperature = -5;
  const double maxTemperature = 15;
  const unsigned int nSignalHeightBins = 70;
  const double minSignalHeight = 0;
  const double maxSignalHeight = 7000;
  TH2D* histo = new TH2D(qPrintable(title()), "", nTemperatureBins, minTemperature, maxTemperature, nSignalHeightBins, minSignalHeight, maxSignalHeight);
  setAxisTitle("temperature /  #circC", "signal height / adc counts", "");
  m_histo = new TH2D(*histo);
  addHistogram(histo);
  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", nTemperatureBins, minTemperature, maxTemperature);
}

SignalHeightTemperatureCorrelationPlot::~SignalHeightTemperatureCorrelationPlot()
{
  delete m_histo;
  delete m_normHisto;
}

void SignalHeightTemperatureCorrelationPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
{
  const Track* track = particle->track();
  if(!track)
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
      double temperature = event->sensorData(Setup::instance()->sensorForId(hit->elementId()));
      if (std::isnan(temperature))
        return;
      m_histo->Fill(temperature, hit->signalHeight());
      m_normHisto->Fill(temperature);
    }
  }
}

void SignalHeightTemperatureCorrelationPlot::update()
{
  for (int xBin = 1; xBin <= m_histo->GetNbinsX(); xBin++)
    for (int yBin = 1; yBin <= m_histo->GetNbinsY(); yBin++)
      histogram()->SetBinContent(xBin, yBin, m_histo->GetBinContent(xBin, yBin));
}

#include "ExternalTrackerSignalHeight2DPlot.hh"

#include "Track.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "AdditionalHit.hh"
#include "ProjectionControlWidget.hh"
#include "SimpleEvent.hh"

#include <QSpinBox>

#include <TH2D.h>
#include <TH1D.h>

#include <iostream>

ExternalTrackerSignalHeight2DPlot::ExternalTrackerSignalHeight2DPlot(unsigned short moduleId)
  : AnalysisPlot(Enums::Testbeam)
  , H2DProjectionPlot()
  , m_moduleId(moduleId)
  , m_histo(0)
  , m_normHisto(0)
{
  controlWidget()->spinBox()->setMaximum(256);
  int counter = 0;
  const unsigned short nChannels = 256;
  for (int i = 0; i < nChannels; ++i) {
    const unsigned short channel = m_moduleId | i;
    m_indexMap[channel] = counter;
    counter++;
  }
  setTitle(QString("external tracker signal height 2d 0x%1").arg(m_moduleId, 0, 16));
  const double minSignalHeight = -3000;
  const double maxSignalHeight = 5000;
  TH2D* histo = new TH2D(qPrintable(title()), "", counter, -0.5, counter - 0.5, (maxSignalHeight - minSignalHeight) / 80, minSignalHeight, maxSignalHeight);
  m_histo = new TH2D(*histo);
  addHistogram(histo);
  for (QMap<unsigned short, int>::iterator it = m_indexMap.begin(); it != m_indexMap.end(); it++) {
    unsigned short id = it.key();
    int channel = it.value();
    xAxis()->SetBinLabel(channel + 1, qPrintable(QString("0x%1").arg(id, 0, 16)));
  }
  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", counter, -0.5, counter - 0.5);

  addRequiredEventFlags(Enums::TrackGood | Enums::AllTrackerLayers);
}

ExternalTrackerSignalHeight2DPlot::~ExternalTrackerSignalHeight2DPlot()
{
  delete m_histo;
  delete m_normHisto;
}

void ExternalTrackerSignalHeight2DPlot::processEvent(const AnalyzedEvent* event)
{
  const SimpleEvent* simpleEvent = event->simpleEvent();
  std::vector<AdditionalHit*>::const_iterator it = simpleEvent->additionalHits().begin();
  std::vector<AdditionalHit*>::const_iterator end = simpleEvent->additionalHits().end();
  for (; it != end; ++it) {
    AdditionalHit* hit = *it;
    const unsigned short id = hit->detId();
    if (hit->type() == AdditionalHit::ExternalTracker && (id & 0xFF00) == m_moduleId) {
//      const Track* track = event->particle()->track();
//      const double z = 533.03; // TODO use coordinates instead of this fixed max z
//      const double externalTrackerWidth = 64; // mm TODO somehow use coordinates
//      if (qAbs(track->x(z)) > externalTrackerWidth / 2.)
//        return;
      m_histo->Fill(m_indexMap[id], hit->signalHeight());
      m_normHisto->Fill(m_indexMap[id]);
    }
  }
}

void ExternalTrackerSignalHeight2DPlot::update()
{
  TH2D* hist = m_histo;
  for (int xBin = 1; xBin <= hist->GetNbinsX(); xBin++) {
    for (int yBin = 1; yBin <= hist->GetNbinsY(); yBin++) {
      double bc = hist->GetBinContent(xBin, yBin);
      histogram()->SetBinContent(xBin, yBin, bc);
    }
  }
}

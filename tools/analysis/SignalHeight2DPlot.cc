#include "SignalHeight2DPlot.hh"

#include "Track.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "ProjectionControlWidget.hh"

#include <QSpinBox>

#include <TH2D.h>
#include <TH1D.h>


#include <iostream>

SignalHeight2DPlot::SignalHeight2DPlot() :
  AnalysisPlot(SignalHeightTracker),
  H2DProjectionPlot(),
  m_normHisto(0)
{
  controlWidget()->spinBox()->setMaximum(200);
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  int counter = 0;
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::tracker) {
      m_indexMap[element->id()] = counter;
      counter++;
    }
  }

  setTitle("signal height 2d");
  TH2D* histo = new TH2D(qPrintable(title()), "", counter, -0.5, counter - 0.5, 100, 0, 4096);
  addHistogram(histo);
  
  for (QMap<unsigned short, int>::iterator it = m_indexMap.begin(); it != m_indexMap.end(); it++) {
    unsigned short id = it.key();
    int channel = it.value();
    xAxis()->SetBinLabel(channel + 1, qPrintable(QString("0x%1").arg(id, 0, 16)));
  }

  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", counter, -0.5, counter - 0.5);
}

SignalHeight2DPlot::~SignalHeight2DPlot()
{
  delete m_normHisto;
}

void SignalHeight2DPlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  if(!track)
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;

  const QVector<Hit*>::const_iterator endIt = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tracker) {
      unsigned short id = hit->detId() - hit->channel();
      histogram()->Fill(m_indexMap[id], hit->signalHeight());
      m_normHisto->Fill(m_indexMap[id]);
    }
  }
}

void SignalHeight2DPlot::finalize()
{
  TH2D* hist = histogram();
  for (int xBin = 1; xBin <= hist->GetNbinsX(); xBin++) {
    for (int yBin = 1; yBin <= hist->GetNbinsY(); yBin++) {
      double bc = hist->GetBinContent(xBin, yBin);
      double norm = m_normHisto->GetBinContent(xBin);
      bc > 10 ? hist->SetBinContent(xBin, yBin, bc/norm) : hist->SetBinContent(xBin, yBin, 0.);
    }
  }
}

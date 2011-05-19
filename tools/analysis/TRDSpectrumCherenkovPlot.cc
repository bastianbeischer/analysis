#include "TRDSpectrumCherenkovPlot.hh"

#include "SimpleEvent.hh"

TRDSpectrumCherenkovPlot::TRDSpectrumCherenkovPlot(unsigned short id, TRDSpectrumType spectrumType, double lowerMom, double upperMom, CherenkovCut cCut, double c1Limit, double c2Limit) :
  TRDSpectrumPlot(id, spectrumType, lowerMom, upperMom),
  m_cherenkovCut(cCut),
  m_cherenkov1Limit(c1Limit),
  m_cherenkov2Limit(c2Limit)
{
}

TRDSpectrumCherenkovPlot::~TRDSpectrumCherenkovPlot()
{
}

void TRDSpectrumCherenkovPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  double c1Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV1);
  double c2Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV2);

  switch (m_cherenkovCut) {
  case None:
    break;
  case BothBelow:
    if (c1Signal >= m_cherenkov1Limit || c2Signal >= m_cherenkov2Limit)
      return;
    break;
  case BothAbove:
    if (c1Signal < m_cherenkov1Limit || c2Signal < m_cherenkov2Limit)
      return;
    break;
  case C1AboveC2Below:
    if (c1Signal < m_cherenkov1Limit || c2Signal >= m_cherenkov2Limit)
      return;
    break;
  case C1BelowC2Above:
    if (c1Signal >= m_cherenkov1Limit || c2Signal < m_cherenkov2Limit)
      return;
    break;
  }

  TRDSpectrumPlot::processEvent(hits, particle, event);
}

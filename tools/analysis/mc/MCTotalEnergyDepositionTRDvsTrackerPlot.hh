#ifndef MCTotalEnergyDepositionTRDvsTrackerPlot_hh
#define MCTotalEnergyDepositionTRDvsTrackerPlot_hh

#include "AnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QMap>

class MCTotalEnergyDepositionTRDvsTrackerPlot : public AnalysisPlot, public GraphPlot
{

public:
  MCTotalEnergyDepositionTRDvsTrackerPlot();
  ~MCTotalEnergyDepositionTRDvsTrackerPlot();

  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);

private:

  QMap <int, TGraph*> m_graphMap;
  int m_colorCounter;

};

#endif /* TotalEnergyDepositionTRDvsTrackerPlot_hh */

#ifndef PMTPlot_hh
#define PMTPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include "SensorTypes.hh"

class TLine;

class PMTPlot :
  public AnalysisPlot,
  public H1DPlot
{

public:
  PMTPlot(SensorTypes::Type);
  ~PMTPlot();
  void draw(TCanvas*);
  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  void update();
private:
  SensorTypes::Type m_type;
  TLine* m_line;
  const static int s_threshold;
  int m_thresholdBin;
};

#endif /* PMTPlot_hh */

#ifndef PMTCorrelationPlot_hh
#define PMTCorrelationPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include "SensorTypes.hh"

class TLine;

class PMTCorrelationPlot :
  public AnalysisPlot,
  public H2DProjectionPlot
{

public:
  PMTCorrelationPlot();
  ~PMTCorrelationPlot();
  
  void draw(TCanvas*);
  void processEvent(const AnalyzedEvent*);
  void update();

  const double m_c1Threshold;
  const double m_c2Threshold;

  TLine* m_line1;
  TLine* m_line2;

  int m_c1FirstBinAbove;
  int m_c2FirstBinAbove;
  int m_c1LowerEdgeFirstBinAbove;
  int m_c2LowerEdgeFirstBinAbove;

};

#endif /* PMTCorrelationPlot_hh */

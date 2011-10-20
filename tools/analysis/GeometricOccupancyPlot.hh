#ifndef GeometricOccupancyPlot_hh
#define GeometricOccupancyPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class GeometricOccupancyPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  explicit GeometricOccupancyPlot(double zPosition);
  virtual ~GeometricOccupancyPlot();
  virtual void processEvent(const AnalyzedEvent*);
private:
  double m_zPosition;
};

#endif

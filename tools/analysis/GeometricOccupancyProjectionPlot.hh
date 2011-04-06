#ifndef GeometricOccupancyProjectionPlot_hh
#define GeometricOccupancyProjectionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class GeometricOccupancyProjectionPlot : public AnalysisPlot, public H1DPlot {
public:
  GeometricOccupancyProjectionPlot(double zPosition);
  virtual ~GeometricOccupancyProjectionPlot();
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
private:
  double m_zPosition;
};

#endif

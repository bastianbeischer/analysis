#ifndef GeometricOccupancyPlot_hh
#define GeometricOccupancyPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class GeometricOccupancyPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  explicit GeometricOccupancyPlot(double zPosition);
  virtual ~GeometricOccupancyPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  double m_zPosition;
};

#endif

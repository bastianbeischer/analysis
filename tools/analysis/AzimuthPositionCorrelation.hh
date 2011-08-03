#ifndef AzimuthPositionCorrelation_hh
#define AzimuthPositionCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class AzimuthPositionCorrelation : public AnalysisPlot, public H2DProjectionPlot
{
public:
  enum Direction {X, Y};
  enum Type {Positive, Negative, All};
  AzimuthPositionCorrelation(Direction, Type);
  ~AzimuthPositionCorrelation();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  Direction m_direction;
  Type m_type;
};

#endif /* AzimuthPositionCorrelation_hh */

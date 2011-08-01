#ifndef AzimuthPositionYCorrelation_hh
#define AzimuthPositionYCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class AzimuthPositionYCorrelation :
  public AnalysisPlot,
  public H2DProjectionPlot
{
public:
  enum Type { Positive, Negative, All};
  AzimuthPositionYCorrelation(Type type);
  ~AzimuthPositionYCorrelation();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  Type m_type;
};

#endif /* AzimuthPositionYCorrelation_hh */

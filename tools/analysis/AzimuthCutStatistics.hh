#ifndef AzimuthPositionXCorrelation_hh
#define AzimuthPositionXCorrelation_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class AzimuthPositionXCorrelation :
  public AnalysisPlot,
  public H2DProjectionPlot
{
public:
  enum Type { Positive, Negative, All};
  AzimuthPositionXCorrelation(Type type);
  ~AzimuthPositionXCorrelation();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
private:
  Type m_type;
};

#endif /* AzimuthPositionXCorrelation_hh */

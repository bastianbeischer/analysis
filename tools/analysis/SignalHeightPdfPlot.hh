#ifndef SignalHeightPdfPlot_hh
#define SignalHeightPdfPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "Hit.hh"
#include "Enums.hh"

class SignalHeightPdfPlot : public AnalysisPlot, public H2DPlot {
public:
  SignalHeightPdfPlot(Hit::ModuleType, Enums::Particle, const QVector<double>& xBins, const QVector<double>& yBins);
  ~SignalHeightPdfPlot();
  void processEvent(const AnalyzedEvent*);
private:
  Hit::ModuleType m_type;
  Enums::Particle m_particle;
};

#endif /* SignalHeightPdfPlot_hh */

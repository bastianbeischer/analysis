#ifndef SignalHeightPdfPlot_hh
#define SignalHeightPdfPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "Hit.hh"
#include "Enums.hh"

class SignalHeightPdfPlot : public AnalysisPlot, public H2DPlot {
public:
  SignalHeightPdfPlot(Hit::ModuleType, Enums::Particle, const QVector<double>& xBins, const QVector<double>& yBins, int layer = -1);
  ~SignalHeightPdfPlot();
  void processEvent(const AnalyzedEvent*);
  Hit::ModuleType type() const {return m_type;}
  Enums::Particle particle() const {return m_particle;}
  int layer() const {return m_layer;}
private:
  Hit::ModuleType m_type;
  Enums::Particle m_particle;
  int m_layer;
};

#endif /* SignalHeightPdfPlot_hh */

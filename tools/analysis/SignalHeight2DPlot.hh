#ifndef SignalHeight2DPlot_hh
#define SignalHeight2DPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include <QMap>

class Particle;
class SimpleEvent;
class TH1D;

class SignalHeight2DPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  SignalHeight2DPlot();
  ~SignalHeight2DPlot();
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void update();
private:
  QMap<unsigned short, int> m_indexMap;
  TH2D* m_histo;
  TH1D* m_normHisto;
};

#endif /* SignalHeight2DPlot_hh */

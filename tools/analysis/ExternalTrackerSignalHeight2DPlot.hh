#ifndef ExternalTrackerSignalHeight2DPlot_hh
#define ExternalTrackerSignalHeight2DPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include <QMap>

class Particle;
class SimpleEvent;
class TH1D;

class ExternalTrackerSignalHeight2DPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  ExternalTrackerSignalHeight2DPlot(unsigned short moduleId);
  ~ExternalTrackerSignalHeight2DPlot();
  void processEvent(const AnalyzedEvent*);
  void update();
protected:
  unsigned short m_moduleId;
  QMap<unsigned short, int> m_indexMap;
  TH2D* m_histo;
  TH1D* m_normHisto;
};

#endif /* ExternalTrackerSignalHeight2DPlot_hh */

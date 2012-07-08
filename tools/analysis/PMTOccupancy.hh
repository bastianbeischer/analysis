#ifndef PMTOccupancy_hh
#define PMTOccupancy_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QMap>

class TH2D;

class PMTOccupancy : public AnalysisPlot, public H2DPlot {
public:
  explicit PMTOccupancy(int id, double zPosition);
  virtual ~PMTOccupancy();
  virtual void processEvent(const AnalyzedEvent*);
private:
  int m_id;
  double m_zPosition;
  QMap<unsigned short, TH2D*> m_histograms;
};

#endif

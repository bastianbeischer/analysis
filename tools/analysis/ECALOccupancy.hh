#ifndef ECALOccupancy_hh
#define ECALOccupancy_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QMap>

class TH2D;

class ECALOccupancy : public AnalysisPlot, public H2DPlot {
public:
  explicit ECALOccupancy();
  virtual ~ECALOccupancy();
  virtual void processEvent(const AnalyzedEvent*);
private:
  double m_zPosition;
  double m_ecalSideWidth;
  QMap<unsigned short, TH2D*> m_histograms;
};

#endif

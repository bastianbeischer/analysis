#ifndef ExternalTrackerOccupancy_hh
#define ExternalTrackerOccupancy_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QMap>

class TH2D;

class ExternalTrackerOccupancy : public AnalysisPlot, public H2DPlot {
public:
  explicit ExternalTrackerOccupancy(const QVector<int>& ids, double zPosition);
  virtual ~ExternalTrackerOccupancy();
  virtual void processEvent(const AnalyzedEvent*);
private:
  double m_zPosition;
  QMap<unsigned short, TH2D*> m_histograms;
};

#endif

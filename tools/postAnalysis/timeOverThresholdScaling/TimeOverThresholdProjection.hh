#ifndef TimeOverThresholdProjection_hh
#define TimeOverThresholdProjection_hh

#include "PostAnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include <QObject>

class PostAnalysisCanvas;

class TimeOverThresholdProjection : public H2DProjectionPlot, public PostAnalysisPlot{
  Q_OBJECT
public:
  TimeOverThresholdProjection(PostAnalysisCanvas*);
  virtual ~TimeOverThresholdProjection();
  virtual void updateProjection();

private:
  void save(unsigned int tofId);
  unsigned int m_tofId;

private slots:
  void save();
  void saveAll();
};

#endif

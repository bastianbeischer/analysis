#ifndef TimeResolutionPlot_hh
#define TimeResolutionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TimeResolutionPlot : public AnalysisPlot, public H1DPlot {
public:
  TimeResolutionPlot(unsigned short, unsigned short, unsigned short, unsigned short);
  virtual ~TimeResolutionPlot();
  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void update();
  virtual void finalize();
private:
  unsigned short m_idTop1;
  unsigned short m_idTop2;
  unsigned short m_idBottom1;
  unsigned short m_idBottom2;
};

#endif

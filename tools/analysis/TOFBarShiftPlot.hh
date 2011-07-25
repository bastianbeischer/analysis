#ifndef TOFBarShiftPlot_hh
#define TOFBarShiftPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

class TOFBarShiftPlot : public AnalysisPlot, public H1DPlot {
public:
  TOFBarShiftPlot(unsigned short, unsigned short, unsigned short, unsigned short);
  virtual ~TOFBarShiftPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void update();
  virtual void finalize();
private:
  unsigned short m_idTop1;
  unsigned short m_idTop2;
  unsigned short m_idBottom1;
  unsigned short m_idBottom2;
};

#endif

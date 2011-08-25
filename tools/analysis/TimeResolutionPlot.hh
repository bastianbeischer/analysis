#ifndef TimeResolutionPlot_hh
#define TimeResolutionPlot_hh

#include "AnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

class TLine;

class TimeResolutionPlot : public AnalysisPlot, public H2DProjectionPlot {
public:
  TimeResolutionPlot(unsigned short, unsigned short, unsigned short, unsigned short);
  virtual ~TimeResolutionPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  virtual void update();
  virtual void draw(TCanvas*);
private:
  unsigned short m_idTop1;
  unsigned short m_idTop2;
  unsigned short m_idBottom1;
  unsigned short m_idBottom2;
  int m_nBins;
  QVector<TLine*> m_lines;
};

#endif

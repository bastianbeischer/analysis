#ifndef ZSquareTRDPlot_hh
#define ZSquareTRDPlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

class TGraphErrors;

class ZSquareTRDPlot : public AnalysisPlot, public H2DPlot {
public:
  explicit ZSquareTRDPlot();
  virtual ~ZSquareTRDPlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
};

#endif

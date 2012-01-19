#ifndef SignalHeightTrackerFitPlot_hh
#define SignalHeightTrackerFitPlot_hh

#include "LikelihoodPDFFitPlot.hh"

class Likelihood;

class SignalHeightTrackerFitPlot : public LikelihoodPDFFitPlot
{
public:
  SignalHeightTrackerFitPlot(Likelihood*, const TH2D*, int bin, bool singleParticle);
  virtual PDFParameters startParameters() const;
public slots:
  void fit();
};

#endif

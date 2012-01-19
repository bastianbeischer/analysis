#ifndef TimeOverThresholdFitPlot_hh
#define TimeOverThresholdFitPlot_hh

#include "LikelihoodPDFFitPlot.hh"

class Likelihood;

class TimeOverThresholdFitPlot : public LikelihoodPDFFitPlot
{
public:
  TimeOverThresholdFitPlot(Likelihood*, const TH2D*, int bin, bool singleParticle);
  virtual PDFParameters startParameters() const;
public slots:
  void fit();
};

#endif

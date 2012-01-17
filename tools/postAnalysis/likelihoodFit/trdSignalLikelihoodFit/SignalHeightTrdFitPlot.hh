#ifndef SignalHeightTrdFitPlot_hh
#define SignalHeightTrdFitPlot_hh

#include "LikelihoodPDFFitPlot.hh"

class Likelihood;

class SignalHeightTrdFitPlot : public LikelihoodPDFFitPlot
{
public:
  SignalHeightTrdFitPlot(Likelihood*, const TH2D*, int);
  virtual PDFParameters startParameters() const;
public slots:
  void fit();
};

#endif

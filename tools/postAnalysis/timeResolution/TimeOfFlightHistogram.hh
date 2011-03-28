#ifndef TimeOfFlightHistogram_hh
#define TimeOfFlightHistogram_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

class TF1;
class TH2D;

class TimeOfFlightHistogram : public PostAnalysisPlot, public H1DPlot {
public:
  TimeOfFlightHistogram(TCanvas*, int ch);
  virtual ~TimeOfFlightHistogram();
  static TH2D* histgram(TCanvas*);
};

#endif

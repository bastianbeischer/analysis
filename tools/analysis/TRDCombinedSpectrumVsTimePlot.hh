#ifndef TRDCombinedSpectrumVsTimePlot_hh
#define TRDCombinedSpectrumVsTimePlot_hh

#include "PlotCollection.hh"

class QDateTime;

class TRDCombinedSpectrumVsTimePlot :
  public PlotCollection
{
  
public:
  TRDCombinedSpectrumVsTimePlot(const QDateTime&, const QDateTime&);
  ~TRDCombinedSpectrumVsTimePlot();

};

#endif /* TRDCombinedSpectrumVsTimePlot_hh */

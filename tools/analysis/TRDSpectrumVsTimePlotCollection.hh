#ifndef TRDSpectrumVsTimePlotCollection_hh
#define TRDSpectrumVsTimePlotCollection_hh

#include "PlotCollection.hh"

class QDateTime;

class TRDSpectrumVsTimePlotCollection : public PlotCollection
{
public:
  TRDSpectrumVsTimePlotCollection(const QDateTime&, const QDateTime&);
  ~TRDSpectrumVsTimePlotCollection();
};

#endif /* TRDSpectrumVsTimePlotCollection_hh */

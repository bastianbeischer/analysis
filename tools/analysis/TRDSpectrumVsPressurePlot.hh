#ifndef TRDSpectrumVsPressurePlot_hh
#define TRDSpectrumVsPressurePlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "SensorTypes.hh"

#include "TRDSpectrumPlot.hh"

#include <QVector>
#include <QMap>

class TRDSpectrumVsPressurePlot: public AnalysisPlot, public H2DPlot {

public:
  TRDSpectrumVsPressurePlot(unsigned short = 0, TRDSpectrumPlot:: TRDSpectrumType = TRDSpectrumPlot::completeTRD, double lowerMomentum = -100, double upperMomentum = 100 );
  ~TRDSpectrumVsPressurePlot();

  virtual void processEvent(const AnalyzedEvent*);

private:
  unsigned short m_id;
  TRDSpectrumPlot::TRDSpectrumType m_spectrumType;

  const double m_lowerMomentum;
  const double m_upperMomentum;

};

#endif // TRDSpectrumVsPressurePlot_hh

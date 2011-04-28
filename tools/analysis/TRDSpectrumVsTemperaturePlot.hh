#ifndef TRDSpectrumVSTemperaturePlot_hh
#define TRDSpectrumVSTemperaturePlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "SensorTypes.hh"

#include <QVector>
#include <QMap>

class TRDSpectrumVsTemperaturePlot: public AnalysisPlot, public H2DPlot {

public:
  enum TRDSpectrumType{completeTRD, module, channel};


  TRDSpectrumVsTemperaturePlot(unsigned int, TRDSpectrumType, double lowerMomentum = -10, double upperMomentum = 10 );
  ~TRDSpectrumVsTemperaturePlot();

  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);

private:
  unsigned short m_id;
  TRDSpectrumType m_spectrumType;

  const double m_lowerMomentum;
  const double m_upperMomentum;

};

#endif // TRDSPECTRUMVSTEMPERATUREPLOT_H

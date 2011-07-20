#ifndef TRDSpectrumVSTemperaturePlot_hh
#define TRDSpectrumVSTemperaturePlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "SensorTypes.hh"

#include "TRDSpectrumPlot.hh"

#include <QVector>
#include <QMap>

class TRDSpectrumVsTemperaturePlot: public AnalysisPlot, public H2DPlot {

public:
  TRDSpectrumVsTemperaturePlot(unsigned short = 0, TRDSpectrumPlot::TRDSpectrumType = TRDSpectrumPlot::completeTRD);
  ~TRDSpectrumVsTemperaturePlot();

  virtual void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);

private:
  unsigned short m_id;
  TRDSpectrumPlot::TRDSpectrumType m_spectrumType;
};

#endif // TRDSPECTRUMVSTEMPERATUREPLOT_H

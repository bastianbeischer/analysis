#ifndef TRDSpectrumVsTimePlot_hh
#define TRDSpectrumVsTimePlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"
#include "TRDSpectrumPlot.hh"

#include <QVector>
#include <QMap>
#include <QDateTime>

class TRDSpectrumVsTimePlot: public AnalysisPlot, public H2DPlot {

public:
  TRDSpectrumVsTimePlot(QDateTime, QDateTime, unsigned short id = 0, TRDSpectrumPlot::TRDSpectrumType spectrumType = TRDSpectrumPlot::completeTRD);
  ~TRDSpectrumVsTimePlot();

  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);

private:
  unsigned short m_id;
  TRDSpectrumPlot::TRDSpectrumType m_spectrumType;
};

#endif


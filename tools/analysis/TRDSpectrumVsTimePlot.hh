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
  TRDSpectrumVsTimePlot(unsigned short, TRDSpectrumPlot::TRDSpectrumType, QDateTime, QDateTime, double = -100, double = 100 );
  ~TRDSpectrumVsTimePlot();

  void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);

  private:
  unsigned short m_id;
  TRDSpectrumPlot::TRDSpectrumType m_spectrumType;

  const double m_lowerMomentum;
  const double m_upperMomentum;

};

#endif


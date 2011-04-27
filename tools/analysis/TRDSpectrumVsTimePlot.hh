#ifndef TRDSpectrumVsTimePlot_hh
#define TRDSpectrumVsTimePlot_hh

#include "AnalysisPlot.hh"
#include "H2DPlot.hh"

#include <QVector>
#include <QMap>
#include <QDateTime>

class TRDSpectrumVsTimePlot: public AnalysisPlot, public H2DPlot {

public:
  enum TRDSpectrumType{completeTRD, module, channel};

  TRDSpectrumVsTimePlot(unsigned short, TRDSpectrumType, QDateTime first, QDateTime last,
                        double lowerMomentum = -10, double upperMomentum = 10 );
  ~TRDSpectrumVsTimePlot();

  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
//  virtual void finalize();
//  virtual void update();
//  virtual void draw(TCanvas*);

  private:
  unsigned short m_id;
  TRDSpectrumType m_spectrumType;

  const double m_lowerMomentum;
  const double m_upperMomentum;

};

#endif


#ifndef TRDTimeCorrectionPlot_hh
#define TRDTimeCorrectionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QDateTime>
#include <QMap>

class TH1D;
class TSpline3;
class TLine;
class Corrections;

class TRDTimeCorrectionPlot : public AnalysisPlot, public H1DPlot {
public:
  TRDTimeCorrectionPlot(QDateTime first, QDateTime last);
  virtual ~TRDTimeCorrectionPlot();
  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  virtual void update();
  virtual void draw(TCanvas* canvas);
protected:
  int m_t1;
  int m_t2;
  TLine* m_line1;
  TLine* m_line2;
  Corrections* m_corr;
  QMap<double, double> m_TrdTimeFactors;
  TSpline3* m_TrdTimeSpline;
};

#endif

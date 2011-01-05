#ifndef TRDFitPlot_hh
#define TRDFitPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include <TF1.h>

class TRDFitPlot : public AnalysisPlot, public H1DPlot
{
  
public:
  TRDFitPlot(AnalysisPlot::Topic, QString title);
  ~TRDFitPlot();

  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();

  void addLandauFit(TF1*);
  void draw(TCanvas* c);

private:
  void updateHistogram();

  QList<TF1*> m_landauFits;

};

#endif /* TRDFitPlot_hh */

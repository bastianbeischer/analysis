#ifndef TRDSpectrumPlot_hh
#define TRDSpectrumPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include <TF1.h>
#include <TMarker.h>

#include <QString>

class TRDSpectrumPlot : public AnalysisPlot, public H1DPlot
{
  
public:
  enum TRDSpectrumType{completeTRD, module, channel};

  TRDSpectrumPlot(unsigned short, TRDSpectrumType);
  ~TRDSpectrumPlot();

  virtual void processEvent(const QVector<Hit*>&, Particle* = 0, SimpleEvent* = 0);
  virtual void finalize();
  virtual void update();
  virtual void draw(TCanvas*);

  static const bool calculateLengthInTube;
  static const int spectrumDefaultBins;

  static double spectrumUpperLimit() {return TRDSpectrumPlot::calculateLengthInTube ? 15 : 50 ;}
  static QString xAxisTitle() {return TRDSpectrumPlot::calculateLengthInTube ? "energy deposition length in tube / (keV/mm)" : "energy deposition / (1/mm)" ;}

  static bool globalTRDCUts(const QVector<Hit*>&, Particle* particle, SimpleEvent* event);

private:
  static const unsigned int m_minTRDLayerCut;

  unsigned short m_id;
  TRDSpectrumType m_spectrumType;

  TF1* m_landauFit;
  const double m_landauFitRange_lower;
  const double m_landauFitRange_upper;

  TMarker* m_fitRangeMarker_lower;
  TMarker* m_fitRangeMarker_upper;

};

#endif /* TRDSpectrumPlot_hh */

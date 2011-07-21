#ifndef AzimuthUnfolding_hh
#define AzimuthUnfolding_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>

#include <TH2D.h>
#include <TH1D.h>
#include <TGraph.h>

class AzimuthUnfolding : public PostAnalysisPlot, public H1DPlot {
public:
  AzimuthUnfolding(TH2D* migrationHistogram, TH1D* histogramToUnfold);
  virtual ~AzimuthUnfolding();
  TH2D* rohIj() {return m_rhoIj;}
  TH1D* unfoldedHistogram() {return m_unfoldedHistogram;}
  TGraph* lCurve() {return m_lCurve;}

private:
  TH2D* m_migrationHistogram;
  TH1D* m_histogramToUnfold;

  void unfold();
  TH1D* m_rawUnfoldedHistogram;
  TH1D* m_unfoldedHistogram;
  TH2D* m_rhoIj;
  TGraph* m_lCurve;

};

#endif

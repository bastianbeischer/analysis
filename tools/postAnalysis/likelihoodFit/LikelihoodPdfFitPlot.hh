#ifndef LikelihoodPdfFitPlot_hh
#define LikelihoodPdfFitPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

class Likelihood;
class TH2D;
class LikelihoodPDF;

class LikelihoodPdfFitPlot : public PostAnalysisPlot, public H1DPlot
{
public:
  LikelihoodPdfFitPlot(const Likelihood*, const TH2D*, int bin);
  ~LikelihoodPdfFitPlot();
private:
  LikelihoodPDF* m_currentFunction;
  LikelihoodPDF* m_previewFunction;
};

#endif

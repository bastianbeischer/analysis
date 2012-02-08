#ifndef LikelihoodPDFPlot_hh
#define LikelihoodPDFPlot_hh

#include <QObject>

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

class Likelihood;
class LikelihoodPDF;

class LikelihoodPDFPlot : public QObject, public PostAnalysisPlot, public H1DPlot
{
Q_OBJECT
public:
  LikelihoodPDFPlot(Enums::LikelihoodVariable, Enums::Particles, int layer = -1);
  ~LikelihoodPDFPlot();
private slots:
  void rigidityChanged(double);
private:
  Likelihood* m_likelihood;
  QMap<Enums::Particle, LikelihoodPDF*> m_pdfs;
};

#endif

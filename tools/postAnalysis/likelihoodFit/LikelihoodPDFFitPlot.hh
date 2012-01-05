#ifndef LikelihoodPDFFitPlot_hh
#define LikelihoodPDFFitPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

#include <QObject>
#include <QVector>

class Likelihood;
class TH2D;
class LikelihoodPDF;
class ParameterWidget;

class LikelihoodPDFFitPlot : public QObject, public PostAnalysisPlot, public H1DPlot
{
Q_OBJECT
public:
  LikelihoodPDFFitPlot(const Likelihood*, const TH2D*, int bin);
  ~LikelihoodPDFFitPlot();
public slots:
  virtual void update();
private:
  LikelihoodPDF* m_currentFunction;
  LikelihoodPDF* m_previewFunction;
  ParameterWidget* m_normalizationFactorWidget;
  QVector<ParameterWidget*> m_parameterWidgets;
};

#endif

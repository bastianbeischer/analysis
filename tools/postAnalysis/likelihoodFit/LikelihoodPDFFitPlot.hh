#ifndef LikelihoodPDFFitPlot_hh
#define LikelihoodPDFFitPlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"
#include "PDFParameters.hh"

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
  LikelihoodPDFFitPlot(Likelihood*, const TH2D*, int bin);
  ~LikelihoodPDFFitPlot();
  virtual PDFParameters startParameters() const = 0;
  int ndf() const;
  double chi2() const;
  virtual bool bad() const;
  virtual bool good() const;
  virtual bool lowStatistics() const;
signals:
  void configFileChanged();
public slots:
  virtual void fit() = 0;
  void update();
  void reset();
  void save();
protected:
  void setup();
  void updateLatex();

  double m_absoluteMomentum;
  Enums::Particle m_particle;
  Likelihood* m_likelihood;
  LikelihoodPDF* m_currentFunction;
  LikelihoodPDF* m_previewFunction;
  ParameterWidget* m_scaleWidget;
  QVector<ParameterWidget*> m_parameterWidgets;
};

#endif

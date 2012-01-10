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
  LikelihoodPDFFitPlot(Likelihood*, const TH2D*, int bin);
  ~LikelihoodPDFFitPlot();
signals:
  void currentChanged();
public slots:
  void update();
  void fit();
  void reset();
  void save();
protected:
  void setup();

  double m_absoluteMomentum;
  Enums::Particle m_particle;
  Likelihood* m_likelihood;
  LikelihoodPDF* m_currentFunction;
  LikelihoodPDF* m_previewFunction;
  ParameterWidget* m_scaleWidget;
  QVector<ParameterWidget*> m_parameterWidgets;
};

#endif

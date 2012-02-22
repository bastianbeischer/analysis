#ifndef LikelihoodFitWindow_h
#define LikelihoodFitWindow_h

#include "PostAnalysisWindow.hh"
#include "Enums.hh"
#include "KineticVariable.hh"

#include <QMutex>
#include <QVector>
#include <QPointF>

class LikelihoodPDFFitPlot;
class PostAnalysisPlot;
class QComboBox;
class Likelihood;
class NormalizationThread;
class ParameterPlot;

class LikelihoodFitWindow : public PostAnalysisWindow {
Q_OBJECT
public:
  LikelihoodFitWindow(QWidget* parent = 0);
  virtual ~LikelihoodFitWindow();
public slots:
  void configFileChanged();
  void fitAll();
  void normalizeAll();
  void save();
protected slots:
  void plotSelectionChanged();
  void normalizationFinished();
  void clear();
  virtual void setupAnalysis() = 0;
protected:
  typedef QVector<QPointF> ResultVector;
  static bool lessThan(const QPointF&, const QPointF&);
  bool passes(const LikelihoodPDFFitPlot*);
  QVector<LikelihoodPDFFitPlot*> m_fitPlots;
  QVector<LikelihoodPDFFitPlot*> m_allParticleFitPlots;
  QVector<ParameterPlot*> m_parameterPlots;
  QVector<PostAnalysisPlot*> m_otherPlots;
  QVector<Enums::Particle> m_particleVector;
  QComboBox* m_comboBox;
  QMap<Enums::Particle, Likelihood*> m_likelihoods;
  QMap<Enums::Particle, ResultVector> m_results;
  int m_runningThreadsCounter;
};

#endif

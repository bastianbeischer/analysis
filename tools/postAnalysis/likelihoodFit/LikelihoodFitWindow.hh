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

class LikelihoodFitWindow : public PostAnalysisWindow {
Q_OBJECT
public:
  LikelihoodFitWindow(QWidget* parent = 0);
  ~LikelihoodFitWindow();
public slots:
  void configFileChanged();
  void fitAll();
  void normalizeAll();
  void save();
protected slots:
  void plotSelectionChanged();
  void normalizationFinished();
protected:
  typedef QVector<QPointF> ResultVector;
  static bool lessThan(const QPointF&, const QPointF&);
  virtual void setupAnalysis() = 0;
  bool passes(const LikelihoodPDFFitPlot*);
  Enums::Particles m_particles;
  QVector<LikelihoodPDFFitPlot*> m_fitPlots;
  QVector<PostAnalysisPlot*> m_otherPlots;
  QComboBox* m_comboBox;
  QVector<Likelihood*> m_likelihoods;
  QMap<Enums::Particle, ResultVector> m_results;
  int m_runningThreadsCounter;
};

#endif

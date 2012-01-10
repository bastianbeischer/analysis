#ifndef LikelihoodFitWindow_h
#define LikelihoodFitWindow_h

#include "PostAnalysisWindow.hh"
#include "Enums.hh"

#include <QVector>

class LikelihoodPDFFitPlot;

class LikelihoodFitWindow : public PostAnalysisWindow {
Q_OBJECT
public:
  LikelihoodFitWindow(QWidget* parent = 0);
  ~LikelihoodFitWindow();
public slots:
  void configFileChanged();
  void fitAll();
  void normalizeAll();
  void saveAll();
  void saveGood();
protected:
  virtual void setupAnalysis() = 0;

  Enums::Particles m_particles;
  QVector<LikelihoodPDFFitPlot*> m_fitPlots;
};

#endif

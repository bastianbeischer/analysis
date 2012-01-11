#ifndef LikelihoodFitWindow_h
#define LikelihoodFitWindow_h

#include "PostAnalysisWindow.hh"
#include "Enums.hh"

#include <QVector>

class LikelihoodPDFFitPlot;
class PostAnalysisPlot;
class QComboBox;

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
protected:
  virtual void setupAnalysis() = 0;
  bool passes(const LikelihoodPDFFitPlot*);
  Enums::Particles m_particles;
  QVector<LikelihoodPDFFitPlot*> m_fitPlots;
  QVector<PostAnalysisPlot*> m_otherPlots;
  QComboBox* m_comboBox;
};

#endif

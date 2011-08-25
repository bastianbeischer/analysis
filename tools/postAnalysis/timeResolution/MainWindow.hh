#ifndef MainWindow_h
#define MainWindow_h

#include "PostAnalysisWindow.hh"

class QComboBox;
class PostAnalysisPlot;

class MainWindow : public PostAnalysisWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
protected:
  virtual void setupAnalysis();
private slots:
  void plotSelectionChanged(int);
private:
  QComboBox* m_plotComboBox;
  QVector<PostAnalysisPlot*> m_plots;
  QVector<PostAnalysisPlot*> m_evaluatedPlots;
  QVector<PostAnalysisPlot*> m_discardedPlots;
  QVector<PostAnalysisPlot*> m_bothPlots;
};

#endif

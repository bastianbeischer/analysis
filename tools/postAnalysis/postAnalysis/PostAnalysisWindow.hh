#ifndef PostAnalysisWindow_h
#define PostAnalysisWindow_h

#include <QMainWindow>

class TCanvas;
class TFile;
class PostAnalysisPlot;
class Ui_postAnalysisWindow;

class PostAnalysisWindow : public QMainWindow {
Q_OBJECT
public:
  PostAnalysisWindow(QWidget* parent = 0);
  ~PostAnalysisWindow();
  void setAnalysisFile(const QString&);
private slots:
  void savePlotsButtonClicked();
  void saveAllPlotsButtonClicked();
  void canvasListWidgetCurrentRowChanged(int);
  void plotListWidgetCurrentRowChanged(int);
  void plotOptionComboBoxCurrentIndexChanged(const QString& option);
protected:
  virtual void setupAnalysis() = 0;
  TCanvas* addCanvas(TFile*, const char* name);
  void addPlot(PostAnalysisPlot*);
  QString m_analysisFile;
  QVector<TCanvas*> m_canvases;
  QVector<PostAnalysisPlot*> m_plots;
private:
  Ui_postAnalysisWindow* m_ui;
};

#endif

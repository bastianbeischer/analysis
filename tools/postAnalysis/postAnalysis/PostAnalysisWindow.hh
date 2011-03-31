#ifndef PostAnalysisWindow_h
#define PostAnalysisWindow_h

#include <QMainWindow>
#include <QListWidgetItem>

class TFile;
class PostAnalysisPlot;
class PostAnalysisCanvas;
class Ui_postAnalysisWindow;

class PostAnalysisWindow : public QMainWindow {
Q_OBJECT
public:
  PostAnalysisWindow(QWidget* parent = 0);
  ~PostAnalysisWindow();
  void setAnalysisFile(const QString&);
protected:
  virtual void setupAnalysis() = 0;
  PostAnalysisCanvas* addCanvas(TFile*, const QString& name);
  void addPlot(PostAnalysisPlot*);
  QString m_analysisFile;
  QVector<PostAnalysisCanvas*> m_canvases;
  QVector<PostAnalysisPlot*> m_plots;
private slots:
  void savePlotsButtonClicked();
  void saveAllPlotsButtonClicked();
  void selectCanvas(QListWidgetItem*);
  void selectCanvas(QListWidgetItem*, QListWidgetItem*);
  void selectPlot(QListWidgetItem*);
  void selectPlot(QListWidgetItem*, QListWidgetItem*);
  void plotOptionComboBoxCurrentIndexChanged(const QString& option);
private:
  Ui_postAnalysisWindow* m_ui;
};

#endif

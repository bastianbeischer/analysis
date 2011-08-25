#ifndef PostAnalysisWindow_h
#define PostAnalysisWindow_h

#include <QMainWindow>
#include <QListWidgetItem>
#include <QStringList>

class TFile;
class PostAnalysisPlot;
class PostAnalysisCanvas;
class Ui_postAnalysisWindow;

class PostAnalysisWindow : public QMainWindow {
Q_OBJECT
public:
  PostAnalysisWindow(QWidget* parent = 0);
  ~PostAnalysisWindow();
  void addAnalysisFiles(const QStringList&);
protected:
  void addWidget(QWidget*);
  virtual void setupAnalysis() = 0;
  PostAnalysisCanvas* addCanvas(TFile*, const QString& name);
  void addPlot(PostAnalysisPlot*);
  void clearPlots();
  QStringList m_analysisFiles;
  QVector<PostAnalysisCanvas*> m_canvases;
  QVector<PostAnalysisPlot*> m_plots;
private slots:
  void saveButtonClicked();
  void saveAllButtonClicked();
  void selectCanvas(QListWidgetItem*);
  void selectCanvas(QListWidgetItem*, QListWidgetItem*);
  void selectPlot(QListWidgetItem*);
  void selectPlot(QListWidgetItem*, QListWidgetItem*);
  void canvasPositionChanged(double, double);
  void plotOptionComboBoxCurrentIndexChanged(const QString& option);
  void aspectRatioChanged(int);
protected:
  Ui_postAnalysisWindow* m_ui;
};

#endif

#ifndef PostAnalysisWindow_h
#define PostAnalysisWindow_h

#include <QMainWindow>
#include <QListWidgetItem>
#include <QFileDialog>
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
  QFileDialog::Options m_dialogOptions;
private slots:
  void canvasFilterChanged(const QString&);
  void plotFilterChanged(const QString&);
  void saveButtonClicked();
  void saveAllButtonClicked();
  void selectCanvas(QListWidgetItem*);
  void selectCanvas(QListWidgetItem*, QListWidgetItem*);
  void selectPlot(QListWidgetItem*);
  void selectPlot(QListWidgetItem*, QListWidgetItem*);
  void canvasPositionChanged(double, double);
  void plotOptionComboBoxCurrentIndexChanged(const QString& option);
  void aspectRatioChanged(int);
  void unzoom();
protected:
  Ui_postAnalysisWindow* m_ui;
  PostAnalysisCanvas* m_selectedCanvas;//int m_selectedPlot;
  PostAnalysisPlot* m_selectedPlot;//int m_selectedPlot;
};

#endif

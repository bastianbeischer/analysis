#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"

#include <QMainWindow>

class QTabBar;

class DataChain;

class MainWindow : public QMainWindow {
  Q_OBJECT
  public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
  void setFileList(const QString&);
  void addFileList(const QString&);
private slots:
  void setOrAddFileListButtonClicked();
  void analyzeButtonClicked();
  void saveCanvasButtonClicked();
  void saveAllCanvasButtonClicked();
  void listWidgetItemChanged(QListWidgetItem*);
  void listWidgetCurrentRowChanged(int);
  void showButtonsClicked();
private:
  void setupAnalysis();
  void startAnalysis();
  void removeListWidgetItem(int);
  void closeEvent(QCloseEvent*);

  Ui_mainWindow m_ui;
  QVector<unsigned int> m_activePlots;
};

#endif

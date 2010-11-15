#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"

class Plotter;
class QTabBar;
class QTimer;

class DataChain;

class MainWindow : public QDialog {
  Q_OBJECT
  public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void setFileList(const QString&);
    void addFileList(const QString&);
  private slots:
    void setOrAddFileListButtonClicked();
    void analyzeButtonClicked();
    void saveButtonClicked();
    void listWidgetItemChanged(QListWidgetItem*);
    void listWidgetCurrentRowChanged(int);
    void showButtonsClicked();
    void mainWindowFinished();
  private:
    void setupAnalysis();
    void startAnalysis();
    void removeListWidgetItem(int);

    Ui_mainWindow m_ui;
    QTimer* m_updateTimer;
    Plotter* m_plotter;
    QVector<unsigned int> m_activePlots;
};

#endif
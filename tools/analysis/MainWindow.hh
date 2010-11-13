#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"

class Plotter;
class QLabel;
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
    void tabBarCloseRequested(int);
    void tabBarCurrentChanged(int);
    void showButtonsClicked();
  private:
    void setupAnalysis();
    void startAnalysis();

    Ui_MainWindow m_ui;
    QTimer* m_updateTimer;
    Plotter* m_plotter;
    QLabel* m_positionLabel;
    QTabBar* m_tabBar;
    QVector<unsigned int> m_activePlots;
};

#endif

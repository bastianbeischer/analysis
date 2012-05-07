#ifndef MainWindow_hh
#define MainWindow_hh

#include "ui_MainWindow.h"

class TH2S;
class CalibrationCollection;
class DetectorElement;
class TH2S;

class MainWindow : public QMainWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
  void setRunFile(const QString&);
private slots:
  void update();
  void saveButtonClicked();
private:
  static bool compareElementPosition(DetectorElement*, DetectorElement*);
  Ui_mainWindow m_ui;
  QString m_fileName;
  TH2S* m_histogram;
  CalibrationCollection* m_collection;
};

#endif

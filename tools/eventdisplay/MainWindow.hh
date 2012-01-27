#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"

#include <QStringList>

class MainWindow : public QMainWindow {
  Q_OBJECT
  public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
  void processArguments(QStringList);
private slots:
  void editEventListButtonClicked();
  void eventSpinBoxValueChanged(int);
  void setFileList(const QString&);
  void addFileList(const QString&);
  void addRootFile(const QString&);
  void update();
  void updateEventSpinBox();
  void setOrAddFileListButtonClicked();
  void saveButtonClicked();
private:
  Ui_MainWindow m_ui;
  QString m_settingFile;
  QVector<int> m_eventList;
};

#endif

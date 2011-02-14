#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"

#include <QDialog>
#include <QString>

class MainWindow : public QDialog {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
  void addFile(const QString& = QString());
private slots:
  void createTexFile();
  void addFilesButtonClicked();
  void createButtonClicked();
  void upButtonClicked();
  int insertButtonClicked();
  void removeButtonClicked();
  void downButtonClicked();
private:
  Ui_mainWindow m_ui;
};

#endif

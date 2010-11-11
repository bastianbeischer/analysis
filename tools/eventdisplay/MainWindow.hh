#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"

class MainWindow : public QDialog {
  Q_OBJECT
  public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void setFileList(const QString& fileList);
    void addFileList(const QString& fileList);
  private slots:
    void update();
    void updateEventSpinBox();
    void setOrAddFileListButtonClicked();
    void saveButtonClicked();
  private:
    Ui_MainWindow m_ui;
};

#endif

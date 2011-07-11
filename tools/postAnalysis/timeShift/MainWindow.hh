#ifndef MainWindow_h
#define MainWindow_h

#include "PostAnalysisWindow.hh"

class MainWindow : public PostAnalysisWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
protected:
  virtual void setupAnalysis();
private slots:
  void saveToConfigFileWithoutBarShift();
  void saveToConfigFileWithBarShift();
private:
  void saveToConfigFile(bool withBarShift);
};

#endif

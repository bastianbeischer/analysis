#ifndef MainWindow_h
#define MainWindow_h

#include "PostAnalysisWindow.hh"

class QStringList;

class MainWindow : public PostAnalysisWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();

  void setupAnalysis(QStringList args);
private:
  void setupAnalysis(){}
};

#endif

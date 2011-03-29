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

};

#endif

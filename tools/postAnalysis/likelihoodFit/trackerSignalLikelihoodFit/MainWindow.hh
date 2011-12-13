#ifndef MainWindow_h
#define MainWindow_h

#include "LikelihoodFitWindow.hh"

class MainWindow : public LikelihoodFitWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
protected:
  virtual void setupAnalysis();
};

#endif

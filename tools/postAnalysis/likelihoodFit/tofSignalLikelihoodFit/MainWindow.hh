#ifndef MainWindow_h
#define MainWindow_h

#include "LikelihoodFitWindow.hh"

class Likelihood;
class TimeOverThresholdFitPlot;

class MainWindow : public LikelihoodFitWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  virtual ~MainWindow();
protected:
  virtual void setupAnalysis();
};

#endif

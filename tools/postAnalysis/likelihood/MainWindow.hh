#ifndef MainWindow_h
#define MainWindow_h

#include "PostAnalysisWindow.hh"

class TimeOfFlightLikelihood;

class MainWindow : public PostAnalysisWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
protected:
  virtual void setupAnalysis();
private:
  TimeOfFlightLikelihood* m_tofLikelihood;
};

#endif

#ifndef MainWindow_h
#define MainWindow_h

#include "PostAnalysisWindow.hh"

class TrackerMomentumLikelihood;
class TimeOfFlightLikelihood;

class MainWindow : public PostAnalysisWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
protected:
  virtual void setupAnalysis();
private:
  TrackerMomentumLikelihood* m_trackerLikelihood;
  TimeOfFlightLikelihood* m_tofLikelihood;
};

#endif

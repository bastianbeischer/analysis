#ifndef MainWindow_h
#define MainWindow_h

#include "PostAnalysisWindow.hh"

#include <QVector>

class TrackerMomentumLikelihood;
class TimeOfFlightLikelihood;
class SignalHeightTrdLikelihood;

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
  QVector<SignalHeightTrdLikelihood*> m_signalHeightTrdLikelihoods;
};

#endif

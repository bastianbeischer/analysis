#ifndef MainWindow_h
#define MainWindow_h

#include "LikelihoodFitWindow.hh"

class Likelihood;

class MainWindow : public LikelihoodFitWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
protected:
  virtual void setupAnalysis();
private:
  QVector<const Likelihood*> m_likelihoods;
};

#endif

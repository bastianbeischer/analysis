#ifndef MainWindow_h
#define MainWindow_h

#include "LikelihoodFitWindow.hh"

#include <QVector>
#include <QSpinBox>

class SignalHeightTrdLikelihood;

class MainWindow : public LikelihoodFitWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
protected:
  virtual void setupAnalysis();
  QSpinBox* m_layerSpinBox;
};

#endif

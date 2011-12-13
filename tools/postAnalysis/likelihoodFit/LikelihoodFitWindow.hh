#ifndef LikelihoodFitWindow_h
#define LikelihoodFitWindow_h

#include "PostAnalysisWindow.hh"
#include "Enums.hh"

class LikelihoodFitWindow : public PostAnalysisWindow {
Q_OBJECT
public:
  LikelihoodFitWindow(QWidget* parent = 0);
  ~LikelihoodFitWindow();
protected:
  virtual void setupAnalysis() = 0;
  Enums::Particles m_particles;
};

#endif

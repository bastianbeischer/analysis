#ifndef MoPVScalingPressure_hh
#define MoPVScalingPressure_hh

#include "MoPVScaling.hh"

class QPushButton;

class MoPVScalingPressure : public MoPVScaling
{
  Q_OBJECT
private:
  QPushButton* m_pushButton;
public:
  MoPVScalingPressure(PostAnalysisCanvas* canvas);
public slots:
  virtual void saveDependency();
};

#endif

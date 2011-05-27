#ifndef MoPVScalingTemperature_hh
#define MoPVScalingTemperature_hh

#include "MoPVScaling.hh"

class MoPVScalingTemperature : public MoPVScaling
{
  Q_OBJECT
private:
  QPushButton* m_pushButton;
public:
  MoPVScalingTemperature(PostAnalysisCanvas* canvas);
public slots:
  virtual void saveDependency();
};

#endif

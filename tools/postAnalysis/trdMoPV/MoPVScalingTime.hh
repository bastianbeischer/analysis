#ifndef MoPVScalingTime_hh
#define MoPVScalingTime_hh

#include "MoPVScaling.hh"

class QPushButton;
class QSpinBox;

class MoPVScalingTime : public MoPVScaling
{
  Q_OBJECT
private:
  QPushButton* m_pushButton;
  QSpinBox* m_spinBox;
public:
  MoPVScalingTime(PostAnalysisCanvas* canvas);
public slots:
  virtual void saveCorrectionFactors();
};

#endif

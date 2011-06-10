#ifndef MoPVScalingTime_hh
#define MoPVScalingTime_hh

#include "MoPVScaling.hh"

class QPushButton;
class QDoubleSpinBox;

class MoPVScalingTime : public MoPVScaling
{
  Q_OBJECT
private:
  QPushButton* m_pushButton;
  QDoubleSpinBox* m_spinBox;
public:
  MoPVScalingTime(PostAnalysisCanvas* canvas);
public slots:
  virtual void saveCorrectionFactors();
};

#endif

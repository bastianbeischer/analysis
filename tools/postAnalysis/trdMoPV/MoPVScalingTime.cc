#include "MoPVScalingTime.hh"

#include <Corrections.hh>

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDoubleSpinBox>

#include <TSpline.h>

MoPVScalingTime::MoPVScalingTime(PostAnalysisCanvas* canvas)
  : MoPVScaling(canvas)
  , m_pushButton(0)
{
  //setup widget
  QWidget* secWidget = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(secWidget);
  layout->addStretch();
  m_pushButton = new QPushButton("save time dependend corrections for normalization");
  layout->addWidget(m_pushButton);
  layout->addStretch();
  m_spinBox = new QDoubleSpinBox();
  m_spinBox->setRange(0.1, 3);
  m_spinBox->setSingleStep(0.01);
  m_spinBox->setValue(1);
  layout->addWidget(m_spinBox);
  layout->addStretch();
  setSecondaryWidget(secWidget);
  connect(m_pushButton, SIGNAL(pressed()), this, SLOT(saveCorrectionFactors()));
}


void MoPVScalingTime::saveCorrectionFactors()
{
  double norm = m_spinBox->value();
  //get corrections class:
  Corrections* corr = new Corrections();

  //delete old factors, crucial if different time binning is used
  corr->removeTrdTimeDependendFactors(m_startValue, m_endValue);

  //save all time dependend factors
  double time, mopv;
  for (int i = 0; i < m_spline->GetNp(); i++) {
    m_spline->GetKnot(i, time, mopv);
    double oldFactor = corr->trdTimeDependendFactor(time);
    double newFactor = oldFactor * norm/ mopv;
    qDebug("saving for time %f with mopv=%f for norm=%f new scaling factor %f was %f", time, mopv, norm, newFactor, oldFactor);
    corr->addTrdTimeDependendFactor(time, newFactor);
  }

}

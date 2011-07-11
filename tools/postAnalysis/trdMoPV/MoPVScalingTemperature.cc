#include "MoPVScalingTemperature.hh"

#include <Corrections.hh>

#include <TF1.h>

#include <QPair>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>



MoPVScalingTemperature::MoPVScalingTemperature(PostAnalysisCanvas* canvas)
  : MoPVScaling(canvas)
  , m_pushButton(0)
{
  //setup widget
  QWidget* secWidget = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(secWidget);
  layout->addStretch();
  m_pushButton = new QPushButton("save dependency corrections");
  layout->addWidget(m_pushButton);
  layout->addStretch();
  setSecondaryWidget(secWidget);
  connect(m_pushButton, SIGNAL(pressed()), this, SLOT(saveDependency()));
}


void MoPVScalingTemperature::saveDependency()
{
  //calculate get dependency
  QPair<double,double> T0(30, 1);
  T0.second = m_dependencyFit->Eval(T0.first);
  double dM_dT = m_dependencyFit->GetParameter(1);

  //get corrections class:
  Corrections* corr = new Corrections();

  //get old dependency from corrections
  QPair<double,double> T0_old;
  double dM_dT_old;
  corr->getTrdTemperatureDependendFactor(T0_old, dM_dT_old);

  //calculate new dependency taking old dependency into account
  dM_dT = dM_dT_old + dM_dT;

  qDebug("saving for temperature dependency: T0 = (%f, %f), dM/dT = %f",T0.first, T0.second, dM_dT);
  corr->setTrdTemperatureDependendFactor(T0, dM_dT);

}

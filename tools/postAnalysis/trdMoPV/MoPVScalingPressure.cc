#include "MoPVScalingPressure.hh"

#include <Corrections.hh>

#include <TF1.h>

#include <QPair>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>



MoPVScalingPressure::MoPVScalingPressure(PostAnalysisCanvas* canvas)
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


void MoPVScalingPressure::saveDependency()
{
  //calculate get dependency
  QPair<double,double> P0(1095, 1);
  P0.second = m_dependencyFit->Eval(P0.first);
  double dM_dP = m_dependencyFit->GetParameter(1);

  //get corrections class:
  Corrections* corr = new Corrections();

  //get old dependency from corrections
  QPair<double,double> P0_old;
  double dM_dP_old;
  corr->getTrdPressureDependendFactor(P0_old, dM_dP_old);

  //calculate new dependency taking old dependency into account
  dM_dP = dM_dP_old + dM_dP;

  qDebug("saving for pressure dependency: P0 = (%f, %f), dM/dP = %f",P0.first, P0.second, dM_dP);
  corr->setTrdPressureDependendFactor(P0, dM_dP);

}

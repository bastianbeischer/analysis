#include "ProjectionControlWidget.hh"

#include <QHBoxLayout>
#include <QCheckBox>

ProjectionControlWidget::ProjectionControlWidget(QWidget* parent) :
  QWidget(parent)
{
  QHBoxLayout* layout = new QHBoxLayout(this);
  QCheckBox* cb = new QCheckBox("show projection");
  layout->addWidget(cb);
  connect(cb, SIGNAL(stateChanged(int)), this, SIGNAL(showProjectionChanged(int)));
}

ProjectionControlWidget::~ProjectionControlWidget()
{
}


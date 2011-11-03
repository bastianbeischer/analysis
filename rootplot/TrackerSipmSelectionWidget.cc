#include "TrackerSipmSelectionWidget.hh"

#include "StringSpinBox.hh"

#include <QLabel>
#include <QHBoxLayout>

TrackerSipmSelectionWidget::TrackerSipmSelectionWidget(const QStringList& moduleIDs, QWidget* parent) :
  QWidget(parent)
{
  QHBoxLayout* layout = new QHBoxLayout(this);
  StringSpinBox* spinBox = new StringSpinBox(moduleIDs);
  layout->addWidget(new QLabel("sipm: "));
  layout->addWidget(spinBox);
  
  connect(spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(sipmChanged(int)));
}

TrackerSipmSelectionWidget::~TrackerSipmSelectionWidget()
{
}


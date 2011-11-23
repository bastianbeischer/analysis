#include "LocationSelectonWidget.hh"

#include <QCheckBox>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>

LocationSelectonWidget::LocationSelectonWidget(QWidget* parent)
  : QWidget(parent)
  , m_buttonGroup(0)
{
  m_buttonGroup = new QButtonGroup(this);
  for (QMap<Enums::Situation, QString>::ConstIterator it = Enums::situationBegin(); it != Enums::situationEnd(); ++it) {
    m_buttonGroup->addButton(new QCheckBox(it.value(), this));
    if (it.key() == Enums::KirunaFlight)
      m_buttonGroup->buttons().last()->setChecked(true);
  }
  m_buttonGroup->setExclusive(true);
  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(new QLabel("location:"));
  foreach (QAbstractButton* button, m_buttonGroup->buttons()) {
    layout->addWidget(button);
  }
  layout->addStretch();
  connect(m_buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(selectionChanged(QAbstractButton*)));
}

LocationSelectonWidget::~LocationSelectonWidget()
{
}

Enums::Situation LocationSelectonWidget::situation()
{
  const QString& checkBoxText = m_buttonGroup->checkedButton()->text();
  Enums::Situation situation = Enums::situation(checkBoxText);
  return situation;
}

void LocationSelectonWidget::selectionChanged(QAbstractButton*)
{
  emit(selectionChanged(situation()));
}

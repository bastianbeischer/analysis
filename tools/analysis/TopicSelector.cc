#include "TopicSelector.hh"

#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QDebug>

TopicSelector::TopicSelector(Enums::AnalysisTopic topic, QWidget* parent)
  : QWidget(parent)
  , m_topic(topic)
{
  m_checkBox = new QCheckBox(Enums::label(topic));
  
  m_button = new QPushButton("+");
  m_button->setEnabled(false);
  m_button->setMaximumWidth(27);
  m_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QHBoxLayout* layout = new QHBoxLayout;
  layout->addWidget(m_checkBox);
  layout->addWidget(m_button);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  setMaximumHeight(27);

  connect(m_button, SIGNAL(clicked()), this, SLOT(toggleButton()));
}

TopicSelector::~TopicSelector()
{}

Enums::AnalysisTopic TopicSelector::topic() const
{
  return m_topic;
}

bool TopicSelector::isActive() const
{
  return !m_checkBox->isEnabled();
}

void TopicSelector::setActive(bool value)
{
  if (value) {
    m_checkBox->setEnabled(false);
    m_button->setEnabled(m_checkBox->isChecked());
    m_button->setText("+");
  } else {
    m_checkBox->setEnabled(true);
  }
}

void TopicSelector::toggleButton()
{
  if (m_button->text() == "+") {
    m_button->setText("-");
    emit show(m_topic);
  } else {
    m_button->setText("+");
    emit hide(m_topic);
  }
}

bool TopicSelector::checked() const
{
  return m_checkBox->checkState() == Qt::Checked;
}

void TopicSelector::setChecked(bool value)
{
  m_checkBox->setChecked(value ? Qt::Checked : Qt::Unchecked);
}

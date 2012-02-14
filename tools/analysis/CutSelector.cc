#include "CutSelector.hh"

#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
#include <QDebug>

CutSelector::CutSelector(Enums::Cut cut, QWidget* parent)
  : QWidget(parent)
  , m_cut(cut)
{
  QString label(Enums::label(cut));
  label.chop(4);
  m_checkBox = new QCheckBox(label);

  m_minimumEdit = new QLineEdit();
  m_minimumEdit->setPlaceholderText("min");
  m_minimumEdit->setMaximumWidth(50);
  m_minimumEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  m_maximumEdit = new QLineEdit();
  m_maximumEdit->setPlaceholderText("max");
  m_maximumEdit->setMaximumWidth(50);
  m_maximumEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QHBoxLayout* minMaxLayout = new QHBoxLayout;
  minMaxLayout->setContentsMargins(0, 0, 0, 0);
  minMaxLayout->setSpacing(2);
  minMaxLayout->addWidget(m_minimumEdit);
  minMaxLayout->addWidget(m_maximumEdit);

  QHBoxLayout* layout = new QHBoxLayout;
  layout->setContentsMargins(2, 3, 2, 3);
  layout->addWidget(m_checkBox);
  layout->addLayout(minMaxLayout);
  setLayout(layout);
}

CutSelector::~CutSelector()
{}

Cut CutSelector::cut() const
{
  Cut cut(m_cut);
  bool ok;
  double min = minimum(&ok);
  if (ok) cut.setMin(min);
  double max = maximum(&ok);
  if (ok) cut.setMax(max);
  return cut;
}

void CutSelector::setMinimum(double value)
{
  m_minimumEdit->setText(QString::number(value));
}

double CutSelector::minimum(bool* ok) const
{
  return m_minimumEdit->text().toDouble(ok);
}

void CutSelector::setMaximum(double value)
{
  m_maximumEdit->setText(QString::number(value));
}

double CutSelector::maximum(bool* ok) const
{
  return m_maximumEdit->text().toDouble(ok);
}

bool CutSelector::checked() const
{
  return m_checkBox->checkState() == Qt::Checked;
}

void CutSelector::setChecked(bool value)
{
  m_checkBox->setChecked(value ? Qt::Checked : Qt::Unchecked);
}

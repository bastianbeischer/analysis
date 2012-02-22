#include "EnumSelector.hh"

#include "Enums.hh"

#include <QAction>

template <typename T>
EnumSelector<T>::EnumSelector(const QString& text, typename QMap<T, QString>::ConstIterator beginIt, typename QMap<T, QString>::ConstIterator endIt, QWidget* parent)
  : SelectionWidget(text, parent)
  , m_actions()
{
  for (typename QMap<T, QString>::ConstIterator it = beginIt; it != endIt; ++it)
    m_actions.insert(it.key(), addElement(it.value()));
}

template <typename T>
EnumSelector<T>::~EnumSelector()
{
}

template <typename T>
QVector<T> EnumSelector<T>::selection() const
{
  QVector<T> selected;
  foreach (T key, m_actions) {
    Q_ASSERT(m_actions[key]);
    QAction* action = m_actions[key];
    if (action->isChecked())
      selected.append(key);
  }
  return selected;
}

template <typename T>
bool EnumSelector<T>::isSelected(T entry) const
{
  foreach (T key, m_actions) {
    Q_ASSERT(m_actions[key]);
    QAction* action = m_actions[key];
    if (key == entry) {
      if (action->isChecked())
        return true;
      else
        return false;
    }
  }
  return false;
}

template <typename T>
void EnumSelector<T>::select(const QVector<T>& entries, bool deselect)
{
  foreach (T key, m_actions) {
    Q_ASSERT(m_actions[key]);
    QAction* action = m_actions[key];
    if (entries.contains(key))
      action->setChecked(!deselect);
  }
  emit selectionChanged();
}

template <typename T>
void EnumSelector<T>::overwriteText(bool overwrite)
{
  if (overwrite) {
    QString newText;
    foreach (QAction* action, m_actions) {
      if (action->isChecked()) {
        if (!newText.isEmpty())
          newText += " ";
        newText += action->text();
      }
    }
    setText(newText);
  } else {
    setText(m_defaultText);
  }
}

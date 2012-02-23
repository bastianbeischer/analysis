#ifndef EnumSelector_cc
#define EnumSelector_cc

#include "EnumSelector.hh"

template <typename T>
EnumSelector<T>::EnumSelector(const QString& text, typename QMap<T, QString>::ConstIterator beginIt, typename QMap<T, QString>::ConstIterator endIt, QWidget* parent)
: SelectionWidget(text, parent)
, m_actions()
{
  for (typename QMap<T, QString>::ConstIterator it = beginIt; it != endIt; ++it)
    m_actions.insert(it.key(), addElement(it.value()));
}

template <typename T>
EnumSelector<T>::EnumSelector(const QString& text, typename QMap<T, QString>::ConstIterator beginIt, typename QMap<T, QString>::ConstIterator endIt, QVector<T> skim,QWidget* parent)
: SelectionWidget(text, parent)
, m_actions()
{
  for (typename QMap<T, QString>::ConstIterator it = beginIt; it != endIt; ++it)
    if (skim.contains(it.key()))
      m_actions.insert(it.key(), addElement(it.value()));
}

template <typename T>
EnumSelector<T>::EnumSelector(const QString& text, typename QMap<T, QString>::ConstIterator beginIt, typename QMap<T, QString>::ConstIterator endIt, QFlags<T> skim,QWidget* parent)
: SelectionWidget(text, parent)
, m_actions()
{
  for (typename QMap<T, QString>::ConstIterator it = beginIt; it != endIt; ++it)
    if (it.key() & skim)
      m_actions.insert(it.key(), addElement(it.value()));
}

template <typename T>
EnumSelector<T>::~EnumSelector()
{
}

template <typename T>
QVector<T> EnumSelector<T>::content() const
{
  return QVector<T>::fromList(m_actions.keys());
}

template <typename T>
QVector<T> EnumSelector<T>::selectedEnums() const
{
  QVector<T> checked;
  foreach (T key, m_actions.keys()) {
    Q_ASSERT(m_actions[key]);
    QAction* action = m_actions[key];
    if (action->isChecked())
      checked.append(key);
  }
  return checked;
}

template <typename T>
QFlags<T> EnumSelector<T>::selectedEnumFlags() const
{
  return flagsFromVector(selectedEnums());
}

template <typename T>
QVector<T> EnumSelector<T>::unselectedEnums() const
{
  QVector<T> unchecked;
  foreach (T key, m_actions.keys()) {
    Q_ASSERT(m_actions[key]);
    QAction* action = m_actions[key];
    if (!action->isChecked())
      unchecked.append(key);
  }
  return unchecked;
}

template <typename T>
QFlags<T> EnumSelector<T>::unselectedEnumsFlags() const
{
  return flagsFromVector(unselectedEnums());
}

template <typename T>
bool EnumSelector<T>::isSelected(T entry) const
{
  foreach (T key, m_actions.keys()) {
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
void EnumSelector<T>::select(T entry, bool mark)
{
  Q_ASSERT(m_actions[entry]);
  m_actions[entry]->setChecked(mark);
  emit selectionChanged();
}

template <typename T>
void EnumSelector<T>::select(const QVector<T>& entries, bool mark)
{
  foreach (T key, entries)
  select(key, mark);
}

template <typename T>
void EnumSelector<T>::select(QFlags<T> flags, bool mark)
{
  select(vectorFromFlags(flags), mark);
}

template <typename T>
void EnumSelector<T>::setSelected(const QVector<T>& entries, bool mark)
{
  foreach (T key, m_actions.keys()) {
    if (entries.contains(key))
      select(key, mark);
    else
      select(key, !mark);
  }
}

template <typename T>
void EnumSelector<T>::setSelected(QFlags<T> flags, bool mark)
{
  setSelected(vectorFromFlags(flags), mark);
}

template <typename T>
void EnumSelector<T>::unselect(T entry)
{
  select(entry, false);
}

template <typename T>
void EnumSelector<T>::unselect(const QVector<T>& entries)
{
  select(entries, false);
}

template <typename T>
void EnumSelector<T>::setUnselected(const QVector<T>& entries)
{
  setSelected(entries, false);
}

template <typename T>
QFlags<T> EnumSelector<T>::flagsFromVector(const QVector<T>& vector) const
{
  QFlags<T> flags(0);
  foreach (T entry, vector) {
    flags |= entry;
  }
  return flags;
}

template <typename T>
QVector<T> EnumSelector<T>::vectorFromFlags(QFlags<T> flags) const
{
  QVector<T> vector;
  foreach (T key, m_actions.keys()) {
    if (key & flags)
      vector.append(key);
  }
  return vector;
}

#endif

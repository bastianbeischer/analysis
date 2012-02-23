#ifndef EnumSelector_hh
#define EnumSelector_hh

#include <SelectionWidget.hh>

#include <QVector>
#include <QMap>
#include <QString>
#include <QAction>
#include <QFlags>

class QWidget;

template <typename T>
class EnumSelector : public SelectionWidget
{
public:
  typedef typename QMap<T, QString>::ConstIterator EnumIterator;
  EnumSelector(const QString& text, EnumIterator begin, EnumIterator end, QWidget* parent = 0)
    : SelectionWidget(text, parent)
    , m_actions()
  {
    for (EnumIterator it = begin; it != end; ++it)
      m_actions.insert(it.key(), addElement(it.value()));
  }

  EnumSelector(const QString& text, EnumIterator begin, EnumIterator end, QFlags<T> flags, QWidget* parent = 0)
    : SelectionWidget(text, parent)
    , m_actions()
  {
    for (EnumIterator it = begin; it != end; ++it)
      if (it.key() & flags)
        m_actions.insert(it.key(), addElement(it.value()));
  }

  EnumSelector(const QString& text, EnumIterator begin, EnumIterator end, const QVector<T>& elements, QWidget* parent = 0)
    : SelectionWidget(text, parent)
    , m_actions()
  {
    for (EnumIterator it = begin; it != end; ++it)
      if (elements.contains(it.key()))
        m_actions.insert(it.key(), addElement(it.value()));
  }

  ~EnumSelector()
  {
  }

  QFlags<T> elements() const
  {
    return flagsFromVector(elementsVector());
  }

  const QVector<T>& elementsVector() const
  {
    return QVector<T>::fromList(m_actions.keys());
  }

  QFlags<T> selectedElements() const
  {
    return flagsFromVector(selectedElementsVetor());
  }

  QVector<T> selectedElementsVetor() const
  {
    QVector<T> checked;
    for (typename QMap<T, QAction*>::ConstIterator it = m_actions.begin(); it != m_actions.end(); ++it)
      if (it.value()->isChecked())
        checked.append(it.key());
    return checked;
  }

  bool isSelected(T entry) const
  {
    typename QMap<T, QAction*>::ConstIterator it = m_actions.find(entry);
    if (it == m_actions.end())
      return false;
    return it.value()->checked();
  }

  void select(T entry)
  {
    select(entry, true, true);
  }

  void deselect(T entry)
  {
    select(entry, false, true);
  }

  void select(const QVector<T>& entries)
  {
    select(entries, true);
  }

  void deselect(const QVector<T>& entries)
  {
    select(entries, false);
  }

  void select(QFlags<T> flags)
  {
    select(vectorFromFlags(flags));
  }

  void deselect(QFlags<T> flags)
  {
    deselect(vectorFromFlags(flags));
  }

  void setSelectedElements(const QVector<T>& entries)
  {
    foreach (QAction* action, m_actions)
      action->setChecked(false);
    foreach (T entry, entries) {
      typename QMap<T, QAction*>::ConstIterator it = m_actions.find(entry);
      if (it != m_actions.end())
        it.value()->setChecked(true);
    }
    emit selectionChanged();
  }

  void setSelectedElements(QFlags<T> flags)
  {
    setSelectedElements(vectorFromFlags(flags));
  }


private:
  QFlags<T> flagsFromVector(const QVector<T>& vector) const
  {
    QFlags<T> flags(0);
    foreach (T entry, vector)
      flags |= entry;
    return flags;
  }

  QVector<T> vectorFromFlags(QFlags<T> flags) const
  {
    QVector<T> vector;
    foreach (T key, m_actions.keys())
      if (key & flags)
        vector.append(key);
    return vector;
  }

  void select(T entry, bool value, bool emitSignal)
  {
    typename QMap<T, QAction*>::ConstIterator it = m_actions.find(entry);
    Q_ASSERT(it != m_actions.end());
    it.value()->setChecked(value);
    if (emitSignal)
      emit selectionChanged();
  }

  void select(const QVector<T>& entries, bool value)
  {
    foreach (T entry, entries)
      select(entry, value, false);
    emit selectionChanged();
  }

  QMap<T, QAction*> m_actions;
};

#endif

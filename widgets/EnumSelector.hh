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
  EnumSelector(const QString& text, typename QMap<T, QString>::ConstIterator beginIt, typename QMap<T, QString>::ConstIterator endIt, QWidget* parent = 0);
  EnumSelector(const QString& text, typename QMap<T, QString>::ConstIterator beginIt, typename QMap<T, QString>::ConstIterator endIt, QVector<T> skim,QWidget* parent = 0);
  EnumSelector(const QString& text, typename QMap<T, QString>::ConstIterator beginIt, typename QMap<T, QString>::ConstIterator endIt, QFlags<T> skim,QWidget* parent = 0);
  ~EnumSelector();
  QVector<T> content() const;
  QVector<T> selectedEnums() const;
  QFlags<T> selectedEnumFlags() const;
  QVector<T> unselectedEnums() const;
  QFlags<T> unselectedEnumsFlags() const;
  bool isSelected(T entry) const;
  void select(T entry, bool mark = true);
  void select(const QVector<T>& entries, bool mark = true);
  void select(QFlags<T> flags, bool mark = true);
  void setSelected(const QVector<T>& entries, bool mark = true);
  void setSelected(QFlags<T> flags, bool mark = true);
  void unselect(T entry);
  void unselect(const QVector<T>& entries);
  void setUnselected(const QVector<T>& entries);
  void overwriteText(bool overwrite);

private:
  QFlags<T> flagsFromVector(const QVector<T>& vector) const;
  QVector<T> vectorFromFlags(QFlags<T> flags) const;
  QMap<T, QAction*> m_actions;
};

#include "EnumSelector.cc"

#endif

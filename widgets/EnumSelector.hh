#ifndef EnumSelector_hh
#define EnumSelector_hh

#include <SelectionWidget.hh>

#include <QVector>
#include <QMap>
#include <QString>

class QWidget;
class QAction;

template <typename T> 
class EnumSelector : public SelectionWidget
{
public:
  EnumSelector(const QString& text, typename QMap<T, QString>::ConstIterator beginIt, typename QMap<T, QString>::ConstIterator endIt, QWidget* parent = 0);
  ~EnumSelector();
  QVector<T> selection() const;
  bool isSelected(T) const;
  void select(const QVector<T>&, bool deselect = false);
  void overwriteText(bool overwrite = true);
private:
  QMap<T, QAction*> m_actions;
};

#endif

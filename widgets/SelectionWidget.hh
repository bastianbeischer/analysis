#ifndef SelectionWidget_hh
#define SelectionWidget_hh

#include <QPushButton>
#include <QStringList>
#include <QString>

class QWidget;
class QAction;

class SelectionWidget : public QPushButton
{
Q_OBJECT
public:
  SelectionWidget(const QString& text, QWidget* parent = 0);
  ~SelectionWidget();
  void deselectAll();
  void selectAll();
  void clear();
  QStringList selection() const;
  QString selectionText();
  void overwriteText(bool overwrite);
  QAction* addElement(const QString& title);
private:
  QString m_defaultText;
signals:
  void selectionChanged();
};

#endif

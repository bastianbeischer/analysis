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
  void clear();
  QAction* addElement(const QString& title);
  void selectAll();
  void deselectAll();
  QStringList selectedElements() const;
signals:
  void selectionChanged();
private slots:
  void updateToolTip();
private:
  QString toolTipString() const;
};

#endif

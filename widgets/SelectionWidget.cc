#include "SelectionWidget.hh"

#include <QMenu>

SelectionWidget::SelectionWidget(const QString& text, QWidget* parent)
  : QPushButton(text, parent)
  , m_defaultText(text)
{
  QMenu* menu = new QMenu(this);
  setMenu(menu);
}

SelectionWidget::~SelectionWidget()
{
}

void SelectionWidget::deselectAll()
{
  for (int i = 0; i < menu()->actions().size(); ++i) {
    menu()->actions()[i]->setChecked(false);
  }
  emit selectionChanged();
}

void SelectionWidget::selectAll()
{
  for (int i = 0; i < menu()->actions().size(); ++i) {
    menu()->actions()[i]->setChecked(true);
  }
  emit selectionChanged();
}

void SelectionWidget::clear()
{
  foreach(QAction* action, menu()->actions())
    action->disconnect();
  menu()->clear();
  setText(m_defaultText);
  emit selectionChanged();
}

QStringList SelectionWidget::selection() const
{
  QStringList selected;
  for (int i = 0; i < menu()->actions().size(); ++i) {
    QAction* action = menu()->actions()[i];
    if (action->isChecked())
      selected << action->text();
  }
  return selected;
}

QAction* SelectionWidget::addElement(const QString& title)
{
  QAction* action = new QAction(title, menu());
  action->setCheckable(true);
  menu()->addAction(action);
  connect(action, SIGNAL(triggered()), this, SLOT(showMenu()));
  connect(action, SIGNAL(changed()), this, SIGNAL(selectionChanged()));
  return action;
}

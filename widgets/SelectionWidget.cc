#include "SelectionWidget.hh"

#include <QMenu>

SelectionWidget::SelectionWidget(const QString& text, QWidget* parent)
  : QPushButton(text, parent)
{
  QMenu* menu = new QMenu();
  setMenu(menu);
}

SelectionWidget::~SelectionWidget()
{
}

void SelectionWidget::clear()
{
  foreach(QAction* action, menu()->actions())
    action->disconnect();
  menu()->clear();
  emit selectionChanged();
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

void SelectionWidget::selectAll()
{
  foreach (QAction* action, menu()->actions()) {
    action->disconnect();
    action->setChecked(true);
    connect(action, SIGNAL(triggered()), this, SLOT(showMenu()));
    connect(action, SIGNAL(changed()), this, SIGNAL(selectionChanged()));
  }
  emit selectionChanged();
}

void SelectionWidget::deselectAll()
{
  foreach (QAction* action, menu()->actions()) {
    action->disconnect();
    action->setChecked(false);
    connect(action, SIGNAL(triggered()), this, SLOT(showMenu()));
    connect(action, SIGNAL(changed()), this, SIGNAL(selectionChanged()));
  }
  emit selectionChanged();
}

QStringList SelectionWidget::selectedElements() const
{
  QStringList selected;
  foreach (QAction* action, menu()->actions())
    if (action->isChecked())
      selected << action->text();
  return selected;
}


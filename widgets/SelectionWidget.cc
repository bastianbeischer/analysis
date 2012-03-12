#include "SelectionWidget.hh"

#include <QMenu>
#include <QChar>

SelectionWidget::SelectionWidget(const QString& text, QWidget* parent)
  : QPushButton(text, parent)
{
  QMenu* menu = new QMenu();
  setMenu(menu);
  connect(this, SIGNAL(selectionChanged()), this, SLOT(updateToolTip()));
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
  updateToolTip();
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

void SelectionWidget::updateToolTip()
{
  setToolTip(toolTipString());
}

QString SelectionWidget::toolTipString() const
{
  QString elements;
  foreach (QAction* action, menu()->actions()) {
    if (!elements.isEmpty())
      elements += "\n";
    if (action->isChecked())
      elements += QString(QChar(0x2713));
    else
      elements += "   ";
    elements += " ";
    elements += action->text();
  }
  return elements;
}


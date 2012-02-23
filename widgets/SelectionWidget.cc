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

QString SelectionWidget::selectionText()
{
  QString selectionText;
  foreach (QString entry, selection()) {
    if (!selectionText.isEmpty())
      selectionText += ", ";
    selectionText += entry;
  }
  return selectionText;
}

void SelectionWidget::overwriteText(bool overwrite)
{
  if (overwrite) {
    QString newText;
    foreach (QString entry, selection()) {
      if (!newText.isEmpty())
        newText += ", ";
      newText += entry;
    }
    setText(newText);
  } else {
    setText(m_defaultText);
  }
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

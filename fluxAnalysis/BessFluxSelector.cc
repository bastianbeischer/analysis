#include "BessFluxSelector.hh"

#include "RootQtWidget.hh"
#include "H1DPlot.hh"

#include <QDebug>
#include <QLayout>
#include <QComboBox>
#include <QStringList>
#include <QPushButton>
#include <QAction>
#include <QMenu>

BessFluxSelector::BessFluxSelector(int numberOfSelectors, QWidget* parent)
  : QWidget(parent)
  , m_numberOfSelectors(numberOfSelectors)
  , m_layout(0)
  , m_activateButton(0)
  , m_buttons()
  , m_buttonMenus()
  , m_inhibitUpdate(false)
  , m_selectedHistograms()
{
  m_layout = new QHBoxLayout(this);
  m_layout->setContentsMargins(0, 0, 0, 0);
  m_activateButton = new QPushButton("activate BESS");
  m_layout->addWidget(m_activateButton);
  m_layout->addStretch();
  connect(m_activateButton, SIGNAL(clicked()), this, SLOT(activate()));
}

BessFluxSelector::~BessFluxSelector()
{
}

void BessFluxSelector::activate()
{
  m_activateButton->disconnect();
  connect(m_activateButton, SIGNAL(clicked()), this, SLOT(clear()));
  m_activateButton->setText("clear");

  for (int i = 0; i < m_numberOfSelectors; ++i) {
    QMenu* menu = new QMenu(this);
    QPushButton* button = new QPushButton("measurement", this);
    button->setMenu(menu);
    m_layout->addWidget(button);
    m_buttons.append(button);
    m_buttonMenus.append(menu);
    connect(button, SIGNAL(clicked()), this, SLOT(update()));
  }

  for (int i = 0; i < m_numberOfSelectors; ++i)
    fillMenu(m_buttons[i]);
  m_layout->addStretch();

  update();
}

void BessFluxSelector::clear()
{
  m_inhibitUpdate = true;
  for(int iSelector = 0; iSelector < m_numberOfSelectors; ++iSelector) {
    for (int iMenu = 0; iMenu < m_buttonMenus[iSelector]->actions().size(); ++iMenu) {
      m_buttonMenus[iSelector]->actions()[iMenu]->setChecked(false);
    }
  }
  m_inhibitUpdate = false;
  update();
}

void BessFluxSelector::fillMenu(QPushButton* button)
{
  QMenu* menu = button->menu();
  QAction* action = 0;
  foreach (BessFlux::Type type, BessFlux::instance()->types()) {
    QString actionName = BessFlux::typeName(type);
    action = new QAction(actionName, menu);
    action->setCheckable(true);
    menu->addAction(action);
    connect(action, SIGNAL(triggered()), button, SLOT(showMenu()));
    connect(action, SIGNAL(changed()), this, SLOT(update()));
  }
}

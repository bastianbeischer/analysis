#include "BessFluxWidget.hh"

#include "Particle.hh"
#include "BessFlux.hh"

#include <TH1D.h>
#include <QList>
#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QComboBox>

BessFluxWidget::BessFluxWidget(QWidget* parent)
  : BessFluxSelector(2, parent)
{
}

BessFluxWidget::~BessFluxWidget()
{
  qDeleteAll(m_selectedHistograms);
}

void BessFluxWidget::update()
{
  if (m_inhibitUpdate)
    return;
  m_inhibitUpdate = true;
  qDeleteAll(m_selectedHistograms);
  m_selectedHistograms.clear();
  for(int iSelector = 0; iSelector < m_numberOfSelectors; ++iSelector) {
    QList<BessFlux::Type> selectedHistograms;
    for (int iMenu = 0; iMenu < m_buttonMenus[iSelector]->actions().size(); ++iMenu) {
      QAction* action = m_buttonMenus[iSelector]->actions()[iMenu];
      if (action->isChecked()) {
        BessFlux::Type type = BessFlux::instance()->type(action->text());
        if (iSelector == 1)
          selectedHistograms << type;
        else
          m_selectedHistograms.append(BessFlux::instance()->spectrum(type));
      }
    }
    if (iSelector == 1 && selectedHistograms.size())
      m_selectedHistograms.append(BessFlux::instance()->spectrum(selectedHistograms));
  }
  m_inhibitUpdate = false;
  emit selectionChanged();
}

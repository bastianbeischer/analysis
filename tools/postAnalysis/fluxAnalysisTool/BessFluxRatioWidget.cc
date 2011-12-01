#include "BessFluxRatioWidget.hh"

#include "Particle.hh"
#include "BessFlux.hh"

#include <TH1D.h>
#include <QList>
#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QComboBox>

BessFluxRatioWidget::BessFluxRatioWidget(QWidget* parent)
  : BessFluxSelector(2, parent)
{
}

BessFluxRatioWidget::~BessFluxRatioWidget()
{
  qDeleteAll(m_selectedHistograms);
}

void BessFluxRatioWidget::update()
{
  if (m_inhibitUpdate)
    return;
  m_inhibitUpdate = true;
  qDeleteAll(m_selectedHistograms);
  m_selectedHistograms.clear();
  QList<BessFlux::Type> numerator;
  QList<BessFlux::Type> denominator;
  for(int iSelector = 0; iSelector < m_numberOfSelectors; ++iSelector) {
    QList<BessFlux::Type> selectedHistograms;
    for (int iMenu = 0; iMenu < m_buttonMenus[iSelector]->actions().size(); ++iMenu) {
      QAction* action = m_buttonMenus[iSelector]->actions()[iMenu];
      if (action->isChecked()) {
        BessFlux::Type type = BessFlux::type(action->text());
        if (iSelector == 0)
          numerator << type;
        else
          denominator << type;
      }
    }
  }
  if (numerator.size() && denominator.size()) {
    TH1D* numeratorHistogram = BessFlux::instance()->spectrum(numerator);
    TH1D* denominatorHistogram = BessFlux::instance()->spectrum(denominator);
    numeratorHistogram->Divide(denominatorHistogram);
    m_selectedHistograms.append(numeratorHistogram);
    delete denominatorHistogram;
  }
  m_inhibitUpdate = false;
  emit selectionChanged();

}

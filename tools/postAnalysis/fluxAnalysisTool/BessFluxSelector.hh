#ifndef BessFluxSelector_hh
#define BessFluxSelector_hh

#include <QWidget>
#include "BessFlux.hh"

class TCanvas;
class H1DPlot;
class TH1D;
class QMenu;
class QComboBox;
class QPushButton;
class QHBoxLayout;

class BessFluxSelector : public QWidget {
Q_OBJECT
public:
  BessFluxSelector(int numberOfSelectors, QWidget* = 0);
  virtual ~BessFluxSelector();
  const QVector<TH1D*>& selectedHistograms() const {return m_selectedHistograms;}
signals:
  void selectionChanged();
protected slots:
  virtual void update() = 0;
  void clear();
protected:
  int m_numberOfSelectors;
  QHBoxLayout* m_layout;
  QPushButton* m_activateButton;
  QVector<QPushButton*> m_buttons;
  QVector<QMenu*> m_buttonMenus;
  bool m_inhibitUpdate;
  QVector<TH1D*> m_selectedHistograms;
private slots:
  void activate();
private:
  void fillMenu(QPushButton*);
};

#endif

#ifndef SimulationFluxSelector_hh
#define SimulationFluxSelector_hh

#include "SimulationFluxKey.hh"

#include <QWidget>

class TCanvas;
class H1DPlot;
class TH1D;
class QComboBox;
class QPushButton;
class QHBoxLayout;
class QAction;
class SelectionWidget;

class SimulationFluxSelector : public QWidget {
Q_OBJECT
public:
  SimulationFluxSelector(int numberOfSelectors, QWidget* = 0);
  virtual ~SimulationFluxSelector();
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
  QComboBox* m_locationComboBox;
  QComboBox* m_acceptanceComboBox;
  QComboBox* m_sourceComboBox;
  QVector<SelectionWidget*> m_selectors;
  QComboBox* m_phiComboBox;
  bool m_inhibitUpdate;
  QVector<TH1D*> m_selectedHistograms;
private slots:
  void activate();
  void selectPositive();
  void selectNegative();
  void locationChanged();
private:
  void fillLocationComboBox();
  void fillAceptanceComboBox();
  void fillSourceComboBox();
  void fillMenu(SelectionWidget*);
  void fillPhiComboBox();
  void checkParticles(QAction*, bool positive);

  SimulationFluxKey::Location m_location;
};

#endif

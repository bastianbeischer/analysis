#ifndef SimulationFluxSelector_hh
#define SimulationFluxSelector_hh

#include <QWidget>

class TCanvas;
class H1DPlot;
class QMenu;
class QComboBox;
class QPushButton;
class QHBoxLayout;

class SimulationFluxSelector : public QWidget {
Q_OBJECT
public:
  SimulationFluxSelector(int numberOfSelectors, QWidget* = 0);
  virtual ~SimulationFluxSelector();
signals:
  void selectionChanged();
protected slots:
  virtual void update() = 0;
protected:
  int m_numberOfSelectors;
  QHBoxLayout* m_layout;
  QPushButton* m_activateButton;
  QComboBox* m_locationComboBox;
  QComboBox* m_acceptanceComboBox;
  QComboBox* m_sourceComboBox;
  QVector<QPushButton*> m_buttons;
  QVector<QMenu*> m_buttonMenus;
  QComboBox* m_phiComboBox;
  bool m_inhibitUpdate;
private slots:
  void activate();
  void clear();
  void selectPositive();
  void selectNegative();
private:
  void fillLocationComboBox();
  void fillAceptanceComboBox();
  void fillSourceComboBox();
  void fillMenu(QPushButton*);
  void fillPhiComboBox();
};

#endif

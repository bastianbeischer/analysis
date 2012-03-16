#ifndef SimulationFluxSelector_hh
#define SimulationFluxSelector_hh

#include "SimulationFluxKey.hh"
#include "EnumSelector.hh"

#include <QWidget>
#include <QStringList>

class QComboBox;

class SimulationFluxSelector : public QWidget {
Q_OBJECT
public:
  SimulationFluxSelector(const QStringList&, QWidget* = 0);
  virtual ~SimulationFluxSelector();
  QVector<SimulationFluxKey> keys(int i = 0) const;
signals:
  void selectionChanged();
private slots:
  void locationChanged();
private:
  void fillLocationComboBox();
  void fillAcceptanceComboBox();
  void fillSourceComboBox();
  void fillModulationParameterComboBox();
  void fillSelectors();

  QComboBox* m_locationComboBox;
  QComboBox* m_acceptanceComboBox;
  QComboBox* m_sourceComboBox;
  QComboBox* m_modulationParameterComboBox;
  typedef EnumSelector<Enums::Particle> ParticleSelector;
  QVector<ParticleSelector*> m_selectors;
};

#endif

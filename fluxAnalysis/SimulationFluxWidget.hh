#ifndef SimulationFluxWidget_hh
#define SimulationFluxWidget_hh

#include "SimulationFluxReader.hh"
#include "SimulationFlux.hh"
#include "Particle.hh"

#include <TH1D.h>
#include <TCanvas.h>

#include <QString>
#include <QWidget>
#include <QMenu>
#include <QList>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QAction>
#include <QHBoxLayout>
#include <QMap>

class H1DPlot;

class SimulationFluxWidget : public QWidget {

  Q_OBJECT

public:
  SimulationFluxWidget(H1DPlot* plot, TCanvas* canvas, QWidget* = 0);
  ~SimulationFluxWidget();
  bool isActive() const {return m_activated;}

private:
  H1DPlot* m_plot;
  SimulationFluxReader* m_reader;
  TCanvas* m_canvas;

  QPushButton* m_activateButton;
  QHBoxLayout* m_layout;
  QComboBox* m_locationBox;
  QComboBox* m_phiBox;
  QComboBox* m_sourceBox;
  QPushButton* m_individualParticleButton;
  QMenu* m_individualParticleMenu;
  QList<QAction*> m_individualParticleActions;
  QPushButton* m_sumParticleButton;
  QMenu* m_sumParticleMenu;
  QList<QAction*> m_sumParticleActions;
  QPushButton* m_positiveButton;
  QPushButton* m_negativeButton;
  QPushButton* m_clearButton;

  SimulationFluxReader::Location m_location;

  void setPhiBox();
  double m_phi;

  SimulationFluxKey::Source m_source;

  QList<SimulationFluxKey> m_individualParticles;
  QList<SimulationFluxKey> m_individualParticlesChecked;
  void setIndividualParticles();

  QList<SimulationFluxKey> m_sumParticles;
  QList<SimulationFluxKey> m_sumParticlesChecked;
  void setSumParticles();

  QList<TH1D*> m_fluxes;

  bool m_activated;
  void activateControls();
  void deactivateControls();

private slots:
  void activationSlot(bool);

  void setLocation(QString name);
  void setSource(QString name);
  void updateIndividualParticles();
  void setPhi(QString name);
  void updateSumParticles();
  void updateFluxes();
  void updatePlot();
  void clear();
  void clearLegend();
  void clearPlot();
  void selectPositive();
  void selectNegative();

};

#endif

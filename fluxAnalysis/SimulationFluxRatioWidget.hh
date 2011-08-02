#ifndef SimulationFluxRatioWidget_hh
#define SimulationFluxRatioWidget_hh

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

class SimulationFluxRatioWidget : public QWidget {

  Q_OBJECT

public:
  SimulationFluxRatioWidget(H1DPlot* plot, TCanvas* canvas,QWidget* = 0);
  ~SimulationFluxRatioWidget();
  bool isActive() const {return m_activated;}

private:
  H1DPlot* m_plot;
  SimulationFluxReader* m_reader;
  TCanvas* m_canvas;
  int m_nPrevHistograms;
  int m_nPrevLegends;

  QPushButton* m_activateButton;
  QHBoxLayout* m_layout;
  QComboBox* m_locationBox;
  QComboBox* m_phiBox;
  QComboBox* m_sourceBox;
  QPushButton* m_numeratorSumParticleButton;
  QMenu* m_numeratorSumParticleMenu;
  QList<QAction*> m_numeratorSumParticleActions;
  QPushButton* m_denominatorSumParticleButton;
  QMenu* m_denominatorSumParticleMenu;
  QList<QAction*> m_denominatorSumParticleActions;
  QPushButton* m_clearButton;

  SimulationFluxReader::Location m_location;

  void setPhiBox();
  double m_phi;

  SimulationFluxKey::Source m_source;

  QList<SimulationFluxKey> m_numeratorSumParticles;
  QList<SimulationFluxKey> m_numeratorSumParticlesChecked;
  void setNumeratorSumParticles();

  QList<SimulationFluxKey> m_denominatorSumParticles;
  QList<SimulationFluxKey> m_denominatorSumParticlesChecked;
  void setDenominatorSumParticles();

  QList<TH1D*> m_ratios;

  bool m_activated;
  void activateControls();
  void deactivateControls();

private slots:
  void activationSlot(bool);

  void setLocation(QString name);
  void setSource(QString name);
  void updateDenominatorSumParticles();
  void setPhi(QString name);
  void updateNumeratorSumParticles();
  void updateFluxes();
  void updatePlot();
  void clear();
  void clearLegend();
  void clearPlot();

};

#endif

#include "MainWindow.hh"
#include "Enums.hh"
#include "PostAnalysisCanvas.hh"
#include "RigiditySpectrumPlot.hh"
#include "RigidityFluxPlot.hh"
#include "MeasurementTimePlot.hh"
#include "FluxCalculation.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH1.h>

#include <QMap>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis()
{
  TFile file(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();

  MeasurementTimePlot* measurementTimePlot = new MeasurementTimePlot(addCanvas(&file, "measurement time distribution canvas"));
  addPlot(measurementTimePlot);

  QMap<Enums::Particle, const TH1D*> rigiditySpectra;
  foreach (const TH1D* spectrum, addCanvas(&file, "rigidity spectrum canvas")->histograms1D()) {
    QString particleLabel = spectrum->GetName();
    particleLabel.remove("rigidity spectrum ");
    Enums::Particle particle = Enums::particle(particleLabel);
    rigiditySpectra.insert(particle, spectrum);
  }
  
  FluxCalculation::instance()->setRawSpectra(rigiditySpectra);
  FluxCalculation::instance()->setMeasurementTime(measurementTimePlot->measurementTime());

  addPlot(new RigiditySpectrumPlot());
  addPlot(new RigidityFluxPlot());
  //addPlot(new RigidityRatioPlot(calculation));

  file.Close();
}

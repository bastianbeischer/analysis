#include "MainWindow.hh"
#include "Enums.hh"
#include "PostAnalysisCanvas.hh"
#include "RigiditySpectrumPlot.hh"

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
  PostAnalysisCanvas* rigiditySpectrumCanvas = addCanvas(&file, "rigidity spectrum canvas");
  
  QMap<Enums::Particle, const TH1D*> rigiditySpectra;
  foreach (const TH1D* spectrum, rigiditySpectrumCanvas->histograms1D()) {
    QString particleLabel = spectrum->GetName();
    particleLabel.remove("rigidity spectrum ");
    Enums::Particle particle = Enums::particle(particleLabel);
    rigiditySpectra.insert(particle, spectrum);
  }

  RigiditySpectrumPlot* rigiditySpectrumPlot = new RigiditySpectrumPlot(rigiditySpectra);
  addPlot(rigiditySpectrumPlot);

  //RigidityRatioPlot rigidityRatioPlot(rigiditySpectra);
  //addPlot(rigidityRatioSpectra);


  file.Close();
}

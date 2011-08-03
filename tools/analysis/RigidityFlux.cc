#include "RigidityFlux.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Plotter.hh"
#include "RootQtWidget.hh"
#include "SimulationFluxWidget.hh"
#include "Helpers.hh"
#include "EfficiencyCorrectionSettings.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TF1.h>

#include <QVector>
#include <QDebug>

#include <cmath>
#include <vector>

bool RigidityFlux::s_efficienciesLoaded = false;
QMap<RigidityFlux::Type, TH1D*> RigidityFlux::s_multiLayerEff;
QMap<RigidityFlux::Type, TH1D*> RigidityFlux::s_trackFindingEff;

RigidityFlux::RigidityFlux(Type type, const QDateTime& first, const QDateTime& last, Plotter* plotter, TH1D* particleHistogram)
  : AnalysisPlot(AnalysisPlot::MomentumReconstruction)
  , H1DPlot()
  , m_type(type)
  , m_plotter(plotter)
  , m_particleHistogram(particleHistogram)
{
  loadEfficiencies();

  QString title = "flux spectrum";

  m_measurementTimeCalculation = new MeasurementTimeCalculation(first, last);
  m_fluxCalculation = new FluxCalculation(m_particleHistogram, m_measurementTimeCalculation->measurementTime());

  if (m_type == All) {
    title += " - all";
  } else if (m_type == Negative) {
    title += " - negative";
  } else if (m_type == Positive) {
    title += " - positive";
  } else if (m_type == AlbedoPositive) {
    title += " - positive albedo";
  } else if (m_type == AlbedoNegative) {
    title += " - negative albedo";
  }
  setTitle(title);

  TH1D* histogram = m_fluxCalculation->fluxHistogram();

  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
  addHistogram(histogram, H1DPlot::P);

  for(int bin = 0; bin < histogram->GetNbinsX(); bin++)
    addLatex(new TLatex(m_fluxCalculation->binTitle(bin+1)));

  TLegend* legend = new TLegend(.80, .72, .98, .98);
  legend->SetFillColor(kWhite);
  legend->AddEntry(histogram, "Data", "p");
  addLegend(legend);

  m_phiFit = new SolarModulationFit(histogram);
  addFunction(m_phiFit->fit());
  TLatex* gammaLatex = RootPlot::newLatex(.4, .88);
  TLatex* phiLatex = RootPlot::newLatex(.4, .83);
  addLatex(gammaLatex);
  addLatex(phiLatex);

  SimulationFluxWidget* secWidget = new SimulationFluxWidget(this, Plotter::rootWidget()->GetCanvas());
  setSecondaryWidget(secWidget);
}

RigidityFlux::~RigidityFlux()
{
  delete m_measurementTimeCalculation->histogram();
  delete m_measurementTimeCalculation;
  m_measurementTimeCalculation = 0;
  delete m_fluxCalculation;
  m_fluxCalculation = 0;

  QMutex mutex;
  mutex.lock();
  for (int i = 0; i < s_multiLayerEff.size(); i++) {
    Type key = s_multiLayerEff.keys()[i];
    if (s_multiLayerEff[key]) {
      delete s_multiLayerEff[key];
      s_multiLayerEff[key] = 0;
    }
  }
  s_multiLayerEff.clear();

  for (int i = 0; i < s_multiLayerEff.size(); i++) {
    Type key = s_trackFindingEff.keys()[i];
    if (s_trackFindingEff[key]) {
      delete s_trackFindingEff[key];
      s_trackFindingEff[key] = 0;
    }
  }
  s_trackFindingEff.clear();
  s_efficienciesLoaded = false;
  mutex.unlock();
  delete m_phiFit;
  m_phiFit = 0;
}

void RigidityFlux::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  m_measurementTimeCalculation->update(event);
}

void RigidityFlux::update()
{
  m_fluxCalculation->setMeasurementTime(m_measurementTimeCalculation->measurementTime());
  m_fluxCalculation->update();
  efficiencyCorrection();
  updateBinTitles();

  m_phiFit->fit();
  latex(histogram()->GetNbinsX())->SetTitle(qPrintable(m_phiFit->gammaLabel()));
  latex(histogram()->GetNbinsX()+1)->SetTitle(qPrintable(m_phiFit->phiLabel()));
}

void RigidityFlux::finalize()
{
  update();
}

void RigidityFlux::updateBinTitles()
{
  for (int i = 0; i < histogram()->GetNbinsX(); i++) {
    TLatex latext = m_fluxCalculation->binTitle(i+1);
    double x = latext.GetX();
    double y = latext.GetY();
    QString text = latext.GetTitle();
    latex(i)->SetX(x);
    latex(i)->SetY(y);
    latex(i)->SetTitle(qPrintable(text));
  }
}

void RigidityFlux::loadEfficiencies()
{
  QMutex mutex;
  mutex.lock();
  if (!s_efficienciesLoaded) {

    EfficiencyCorrectionSettings effCorSet;

    s_multiLayerEff.insert(All, effCorSet.readHistogram(EfficiencyCorrectionSettings::s_allTrackerLayerCutEfficiencyPreKey+"all/"));
    s_trackFindingEff.insert(All, effCorSet.readHistogram(EfficiencyCorrectionSettings::s_trackFindingEfficiencyPreKey +"all/"));

    s_multiLayerEff.insert(Positive, effCorSet.readHistogram(EfficiencyCorrectionSettings::s_allTrackerLayerCutEfficiencyPreKey+"positive/"));
    s_trackFindingEff.insert(Positive, effCorSet.readHistogram(EfficiencyCorrectionSettings::s_trackFindingEfficiencyPreKey +"positive/"));

    s_multiLayerEff.insert(Negative, effCorSet.readHistogram(EfficiencyCorrectionSettings::s_allTrackerLayerCutEfficiencyPreKey+"negative/"));
    s_trackFindingEff.insert(Negative, effCorSet.readHistogram(EfficiencyCorrectionSettings::s_trackFindingEfficiencyPreKey +"negative/"));

    s_efficienciesLoaded = true;
  }
  mutex.unlock();
}



void RigidityFlux::efficiencyCorrection()
{
  Q_ASSERT(s_multiLayerEff[m_type]);
  m_fluxCalculation->efficiencyCorrection(s_multiLayerEff[m_type]);
  Q_ASSERT(s_trackFindingEff[m_type]);
  m_fluxCalculation->efficiencyCorrection(s_trackFindingEff[m_type]);
  m_fluxCalculation->efficiencyCorrection(0.843684 / 0.792555);
  m_fluxCalculation->efficiencyCorrection(0.999);//estimate for TOF trigger efficiency
}

//double RigidityFlux::maxFlux()
//{
//  double max = histogram(0)->GetBinContent(histogram(0)->GetMaximumBin()) ;
//  for (int i = 0; i <  numberOfHistograms(); i++) {
//    double value = histogram(i)->GetBinContent(histogram(i)->GetMaximumBin());
//    if (value > max) {
//      max = value;
//    }
//  }
//  return max;
//}
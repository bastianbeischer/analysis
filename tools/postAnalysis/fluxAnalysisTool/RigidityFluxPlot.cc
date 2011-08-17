#include "RigidityFluxPlot.hh"

#include "SimulationFluxWidget.hh"
#include "EfficiencyCorrectionSettings.hh"

#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TF1.h>
#include <TLatex.h>
#include <TLegend.h>

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QDebug>
#include <QStringList>
#include <QSettings>
#include <QMutex>

bool RigidityFluxPlot::s_efficienciesLoaded = false;
QMap<RigidityFluxPlot::Type, TH1D*> RigidityFluxPlot::s_multiLayerEff;
QMap<RigidityFluxPlot::Type, TH1D*> RigidityFluxPlot::s_trackFindingEff;

RigidityFluxPlot::RigidityFluxPlot(PostAnalysisCanvas* canvas, double measurementTime)
  : PostAnalysisPlot()
  , H1DPlot()
{
  m_typeNames.insert(All, "all");
  m_typeNames.insert(Positive, "positive");
  m_typeNames.insert(Negative, "negative");
  m_typeNames.insert(AlbedoPositive, "albedoPositive");
  m_typeNames.insert(AlbedoNegative, "albedoNegative");
  loadEfficiencies();

  QString name = canvas->name();
  name.remove(" canvas");
  QString typeName = name;
  typeName.remove("particle spectrum - ");

  m_type = type(typeName);

  m_particleHistogram = new TH1D(*canvas->histograms1D().at(0));
  m_particleHistogram->SetTitle(canvas->histograms1D().at(0)->GetName());

  //corrections and unfolding before
  //....todo

  m_fluxCalculation = new FluxCalculation(m_particleHistogram, measurementTime);

  //corections after flux calculations
  Q_ASSERT(s_multiLayerEff[m_type]);
  m_fluxCalculation->efficiencyCorrection(s_multiLayerEff[m_type]);
  Q_ASSERT(s_trackFindingEff[m_type]);
  m_fluxCalculation->efficiencyCorrection(s_trackFindingEff[m_type]);
  m_fluxCalculation->efficiencyCorrection(0.843684 / 0.792555);
  m_fluxCalculation->efficiencyCorrection(0.999);//estimate for TOF trigger efficiency
  //....todo

  QString title = QString("flux - ") + m_typeNames[m_type];
  setTitle(title);
  addHistogram(m_fluxCalculation->fluxHistogram());
  setAxisTitle(m_fluxCalculation->fluxHistogram()->GetXaxis()->GetTitle(), m_fluxCalculation->fluxHistogram()->GetYaxis()->GetTitle());

  for(int bin = 0; bin < histogram()->GetNbinsX(); bin++)
    addLatex(new TLatex(m_fluxCalculation->binTitle(bin+1)));
  updateBinTitles();

  TLegend* legend = new TLegend(.80, .72, .98, .98);
  legend->SetFillColor(kWhite);
  legend->AddEntry(histogram(), "Data", "p");
  addLegend(legend);

  m_phiFit = new SolarModulationFit(histogram());
  addFunction(m_phiFit->fit());
  TLatex* gammaLatex = RootPlot::newLatex(.4, .88);
  TLatex* phiLatex = RootPlot::newLatex(.4, .83);
  addLatex(gammaLatex);
  addLatex(phiLatex);

  m_phiFit->fit();
  latex(histogram()->GetNbinsX())->SetTitle(qPrintable(m_phiFit->gammaLabel()));
  latex(histogram()->GetNbinsX()+1)->SetTitle(qPrintable(m_phiFit->phiLabel()));

  SimulationFluxWidget* secWidget = new SimulationFluxWidget;
  setSecondaryWidget(secWidget);
}

RigidityFluxPlot::~RigidityFluxPlot()
{
  if (m_particleHistogram) {
    delete m_particleHistogram;
    m_particleHistogram = 0;
  }
  if (m_fluxCalculation) {
    delete m_fluxCalculation;
    m_fluxCalculation = 0;
  }
}

RigidityFluxPlot::Type RigidityFluxPlot::type(const QString& typeName)
{
  for (int i = 0; i < m_typeNames.size(); ++i) {
    Type key = m_typeNames.keys()[i];
    const QString& name = m_typeNames[key];
    if (name == typeName)
      return key;
  }
  Q_ASSERT(m_typeNames.values().contains(typeName));
  return All;
}

void RigidityFluxPlot::updateBinTitles()
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

void RigidityFluxPlot::loadEfficiencies()
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

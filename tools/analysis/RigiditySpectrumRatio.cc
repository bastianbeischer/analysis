#include "RigiditySpectrumRatio.hh"

#include "SimulationFluxRatioWidget.hh"
#include "ParticleInformation.hh"
#include "RootQtWidget.hh"
#include "SimpleEvent.hh"
#include "Constants.hh"
#include "Particle.hh"
#include "Helpers.hh"
#include "Plotter.hh"
#include "Track.hh"

#include <TLegendEntry.h>
#include <TList.h>
#include <TH1D.h>
#include <TPad.h>
#include <TLegend.h>

#include <QVector>
#include <QDebug>

#include <cmath>
#include <vector>

RigiditySpectrumRatio::RigiditySpectrumRatio()
  : AnalysisPlot(Enums::MomentumReconstruction)
  , H1DPlot()
{
  QString title = "rigidity spectrum ratio";
  setTitle(title);

  const int nBins = 21;
  const double min = 0.1;
  const double max = 20;
  const QVector<double>& axis = Helpers::logBinning(nBins, min, max);

  m_numerator = new TH1D("nominator", "", nBins, axis.constData());
  m_numerator->Sumw2();
  m_denominator = new TH1D("denumerator", "", nBins, axis.constData());
  m_denominator->Sumw2();

  TH1D* histogram = new TH1D(qPrintable(title), "", nBins, axis.constData());
  histogram->Sumw2();
  histogram->GetXaxis()->SetTitle("abs(rigidity / GV)");
  histogram->GetYaxis()->SetTitle("ratio #frac{+}{-}");
  histogram->SetLineColor(kBlack);
  histogram->SetMarkerColor(kBlack);
  histogram->SetMarkerStyle(20);

  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
  addHistogram(histogram, H1DPlot::P);

  TLegend* leg = new TLegend(.80, .72, .98, .98);
  leg->SetFillColor(kWhite);
  addLegend(leg);

  legend()->AddEntry(histogram, "Perdaix Data", "p");

  SimulationFluxRatioWidget* secWidget = new SimulationFluxRatioWidget;
  connect(secWidget, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
  setSecondaryWidget(secWidget);
}


RigiditySpectrumRatio::~RigiditySpectrumRatio()
{
  delete m_numerator;
  m_numerator = 0;
  delete m_denominator;
  m_denominator = 0;
}

void RigiditySpectrumRatio::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();
  if (!track || !track->fitGood())
    return;
  ParticleInformation::Flags flags = event->particle()->information()->flags();
  if (!(flags & ParticleInformation::Chi2Good))
    return;
  if (!(flags & ParticleInformation::BetaGood))
    return;
  if (!(flags & ParticleInformation::InsideMagnet))
    return;
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;
  if ((flags & ParticleInformation::Albedo))
    return;

  double rigidity = track->rigidity();
  if (rigidity < 0) {
    m_denominator->Fill(-rigidity);
  } else {
    m_numerator->Fill(rigidity);
  }
}

void RigiditySpectrumRatio::update()
{
  histogram()->Divide(m_numerator, m_denominator);
}

void RigiditySpectrumRatio::selectionChanged()
{
  while (legend()->GetListOfPrimitives()->GetEntries() > 1) {
    TLegendEntry* entry = (TLegendEntry*)legend()->GetListOfPrimitives()->At(1);
    legend()->GetListOfPrimitives()->Remove(entry);
    delete entry;
    entry = 0;
  }
  while(numberOfHistograms() > 1) {
    removeHistogram(1);
  }
  foreach(TH1D* histogram, static_cast<SimulationFluxRatioWidget*>(secondaryWidget())->selectedHistograms()) {
    TH1D* newHisto = new TH1D(*histogram);
    if (!newHisto->GetSumw2())
      newHisto->Sumw2();
    addHistogram(newHisto, H1DPlot::HIST);
    legend()->AddEntry(newHisto, newHisto->GetTitle(), "l");
  }
  draw(Plotter::rootWidget()->GetCanvas());
  gPad->Modified();
  gPad->Update();
}


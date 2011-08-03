#include "RigiditySpectrumRatio.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "SimulationFluxRatioWidget.hh"
#include "Plotter.hh"
#include "RootQtWidget.hh"
#include "Helpers.hh"

#include <TH1D.h>
#include <TLegend.h>

#include <QVector>
#include <QDebug>

#include <cmath>
#include <vector>

RigiditySpectrumRatio::RigiditySpectrumRatio()
  : AnalysisPlot(AnalysisPlot::MomentumReconstruction)
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

  int nBess = 65;

  double bessXaxis[] = {0.576, 0.621, 0.669, 0.72, 0.776, 0.836, 0.901, 0.97, 1.04, 1.13, 1.21, 1.31, 1.41, 1.52, 1.63, 1.76, 1.9, 2.04, 2.2, 2.37, 2.55, 2.75, 2.96, 3.19, 3.44, 3.71, 3.99, 4.3, 4.63, 4.99, 5.38, 5.79, 6.24, 6.73, 7.25, 7.81, 8.41, 9.06, 9.76, 10.5, 11.3, 12.2, 13.2, 14.2, 15.3, 16.4, 17.7, 19.1, 20.6, 23.9, 27.7, 32.1, 37.3, 43.3, 50.2, 58.3, 67.7, 78.5, 91.1, 106, 132, 165, 207, 258, 323 , 404};
  double bessMuPlusFlux[] = {13.4, 13.3, 13.2, 12.7, 12.6, 12.3, 12.1, 11.6, 11, 10.7, 10.2, 9.78, 9.38, 8.72, 8.59, 7.85, 7.41, 7.03, 6.38, 6.01, 5.45, 5.02, 4.62, 4.17, 3.79, 3.37, 3.02, 2.74, 2.41, 2.11, 1.87, 1.63, 1.44, 1.21, 1.06, 0.911, 0.807, 0.706, 0.567, 0.49, 0.438, 0.358, 0.307, 0.249, 0.213, 0.176, 0.149, 0.122, 0.0957, 0.0669, 0.0452, 0.0293, 0.0201, 0.0131, 0.00876, 0.00572, 0.00401, 0.00255, 0.00155, 0.000943, 0.000426, 0.000251, 0.000118, 0.0000558, 0.0000256};
  double bessMuMinusFlux[] = {12.5, 12.2, 11.9, 11.5, 11.3, 10.8, 10.6, 10, 9.6, 9.1, 8.8, 8.53, 8.01, 7.53, 7.22, 6.72, 6.28, 5.71, 5.36, 4.92, 4.62, 4.09, 3.69, 3.31, 3.06, 2.7, 2.37, 2.11, 1.87, 1.66, 1.46, 1.24, 1.13, 0.96, 0.83, 0.693, 0.608, 0.515, 0.454, 0.385, 0.324, 0.274, 0.231, 0.197, 0.168, 0.14, 0.115, 0.096, 0.0736, 0.0504, 0.0348, 0.0226, 0.0155, 0.0104, 0.00691, 0.00454, 0.00288, 0.00193, 0.00117, 0.000695, 0.000365, 0.000147, 0.000075, 0.0000413, 0.0000245};

  double bessMuPlusFluxErrorSys[] = {0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.2, 0.2, 0.2, 0.2, 0.21, 0.2, 0.18, 0.18, 0.17, 0.16, 0.15, 0.13, 0.13, 0.13, 0.12, 0.11, 0.1, 0.09, 0.08, 0.07, 0.06, 0.06, 0.05, 0.04, 0.04, 0.03, 0.03, 0.02, 0.021, 0.019, 0.016, 0.013, 0.011, 0.01, 0.008, 0.007, 0.006, 0.005, 0.004, 0.003, 0.003, 0.0022, 0.0016, 0.001, 0.0007, 0.0005, 0.0003, 0.00022, 0.00015, 0.00011, 0.00008, 0.00005, 0.00004, 0.000022, 0.000015, 0.000009, 0.0000052, 0.000003};
  double bessMuMinusFluxErrorSys[] = {0.3, 0.3, 0.3, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.18, 0.17, 0.16, 0.15, 0.14, 0.13, 0.12, 0.11, 0.1, 0.1, 0.09, 0.08, 0.07, 0.06, 0.06, 0.05, 0.04, 0.04, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.015, 0.013, 0.011, 0.01, 0.008, 0.007, 0.006, 0.005, 0.004, 0.004, 0.003, 0.002, 0.002, 0.0015, 0.0011, 0.0007, 0.0005, 0.0003, 0.0002, 0.00016, 0.00011, 0.00008, 0.00006, 0.00004, 0.000028, 0.000018, 0.000009, 0.000006, 0.0000039, 0.0000028};


  TH1D* bessMuPlus = new TH1D("bess mu+", "", nBess, bessXaxis);
  bessMuPlus->Sumw2();
  TH1D* bessMuMinus = new TH1D("bess mu-", "", nBess, bessXaxis);
  bessMuMinus->Sumw2();
  for (int i = 0; i < nBess; i++) {
    bessMuPlus->SetBinContent(i+1, bessMuPlusFlux[i]);
    bessMuPlus->SetBinError(i+1, bessMuPlusFluxErrorSys[i]);

    bessMuMinus->SetBinContent(i+1, bessMuMinusFlux[i]);
    bessMuMinus->SetBinError(i+1, bessMuMinusFluxErrorSys[i]);
  }

  TH1D* bessMuonRatio = new TH1D("bess muon ratio", "", nBess, bessXaxis);
  bessMuonRatio->Sumw2();
  bessMuonRatio->Divide(bessMuPlus, bessMuMinus);
  bessMuonRatio->SetMarkerColor(kBlue);
  bessMuonRatio->SetLineColor(kBlue);
  addHistogram(bessMuonRatio);
  legend()->AddEntry(bessMuonRatio, "BESS", "p");

  delete bessMuPlus;
  bessMuPlus = 0;
  delete bessMuMinus;
  bessMuMinus = 0;

  SimulationFluxRatioWidget* secWidget = new SimulationFluxRatioWidget(this, Plotter::rootWidget()->GetCanvas());
  setSecondaryWidget(secWidget);
}


RigiditySpectrumRatio::~RigiditySpectrumRatio()
{
  delete m_numerator;
  m_numerator = 0;
  delete m_denominator;
  m_denominator = 0;
}

void RigiditySpectrumRatio::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();
  if (!track || !track->fitGood())
    return;
  ParticleInformation::Flags flags = particle->information()->flags();
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

void RigiditySpectrumRatio::finalize()
{
  update();
}


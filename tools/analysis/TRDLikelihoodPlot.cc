#include "TRDLikelihoodPlot.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TLine.h>

#include <math.h>

#include "TRDElectronIdentifierLikelihood.hh"
#include "SimpleEvent.hh"
#include "Constants.hh"
#include "Settings.hh"
#include "RootQtWidget.hh"

TRDLikelihoodPlot::TRDLikelihoodPlot(Enums::AnalysisTopic topic)
  : AnalysisPlot(topic)
  , H1DPlot()
  , m_NonTRHisto(0)
  , m_TRHisto(0)
  , m_NonTRRejVsTREff(0)
  , m_line50(new TLine)
  , m_line90(new TLine)
  , m_EIdentifierLH(new TRDElectronIdentifierLikelihood())
  , m_NonTRRejVsTREffWidget(new RootQtWidget)
{
  QString topicString;
  if (topic == Enums::MonteCarloTRD)
    topicString = "MonteCarloTRD";
  if (topic == Enums::Testbeam)
    topicString = "Testbeam";
  Q_ASSERT(!topicString.isEmpty());
  setTitle("-log(L) distribution " + topicString);

  m_NonTRHisto = new TH1D(qPrintable(title() + "_nonTR"), "", 500, 0, 3);
  m_TRHisto = new TH1D(qPrintable(title() + "_TR"), "", 500, 0, 3);
  m_TRHisto->SetLineColor(kRed);
  setAxisTitle("-ln(L)", "entries");

  addHistogram(m_NonTRHisto, H1DPlot::HIST);
  addHistogram(m_TRHisto, H1DPlot::HIST);
  setDrawOption(H1DPlot::HIST);

  addLatex(RootPlot::newLatex(0.7, 0.8));
  latex(0)->SetTextColor(kGreen+3);
  addLatex(RootPlot::newLatex(0.7, 0.7));
  latex(1)->SetTextColor(kBlue+2);

  m_line50->SetLineColor(kGreen+3);
  m_line50->SetLineStyle(2);
  m_line50->SetLineWidth(2);

  m_line90->SetLineColor(kBlue+2);
  m_line90->SetLineStyle(2);
  m_line90->SetLineWidth(2);

  m_NonTRRejVsTREff = new TH1D(qPrintable(title() + "_nonTRRejVsTREff"), ";e eff.; bg rej.", 100, 0, 1);

  setSecondaryWidget(m_NonTRRejVsTREffWidget);
}

void TRDLikelihoodPlot::processEvent(const AnalyzedEvent* event)
{
  bool ok = false;
  bool isTRParticle = truthMcIsElectron(event, ok);
  if (!ok)
    return;

  double lhTR = 0.;
  bool validEvent = false;
  m_EIdentifierLH->isElectronish(event, validEvent, lhTR);

  if (!validEvent)
    return;

  if (isTRParticle)
    m_TRHisto->Fill(lhTR);
  else
    m_NonTRHisto->Fill(lhTR);
}

void TRDLikelihoodPlot::update()
{
  double eEff50 = 0;
  double eEff50Err = 0;
  int iBineEff50 = binEfficiency(0.5, m_TRHisto, eEff50, eEff50Err);
  double pRej50Err = 0;
  double pRej50 = rejection(m_NonTRHisto, iBineEff50, pRej50Err);

  double eEff90 = 0;
  double eEff90Err = 0;
  int iBineEff90 = binEfficiency(0.9, m_TRHisto, eEff90, eEff90Err);
  double pRej90Err = 0;
  double pRej90 = rejection(m_NonTRHisto, iBineEff90, pRej90Err);

  latex(0)->SetTitle(qPrintable(QString("#splitline{bg_{rej}(e_{eff} = %1)}{ = %2}")
                                .arg(QString::number(eEff50, 'g', 3))
                                .arg(QString::number(pRej50, 'g', 3))));
  latex(1)->SetTitle(qPrintable(QString("#splitline{bg_{rej}(e_{eff} = %1)}{ = %2}")
                                .arg(QString::number(eEff90, 'g', 3))
                                .arg(QString::number(pRej90, 'g', 3))));

  double yMax = qMax(histogram(0)->GetMaximum(), histogram(1)->GetMaximum()) *1.00;

  m_line50->SetX1(histogram()->GetBinLowEdge(iBineEff50+1));
  m_line50->SetX2(histogram()->GetBinLowEdge(iBineEff50+1));
  m_line50->SetY1(0);
  m_line50->SetY2(yMax);

  m_line90->SetX1(histogram()->GetBinLowEdge(iBineEff90+1));
  m_line90->SetX2(histogram()->GetBinLowEdge(iBineEff90+1));
  m_line90->SetY1(0);
  m_line90->SetY2(yMax);

  updateNonTRRejVsTREffHisto();
}

void TRDLikelihoodPlot::updateNonTRRejVsTREffHisto()
{
  for (int iBin = 1; iBin < m_NonTRRejVsTREff->GetNbinsX(); ++iBin) {
    double eEffWanted = m_NonTRRejVsTREff->GetBinCenter(iBin);
    double eEff = 0;
    double eEffErr = 0;
    int eEffBin = binEfficiency(eEffWanted, m_TRHisto, eEff, eEffErr);
    double bgRejErr = 0;
    double bgRej = rejection(m_NonTRHisto, eEffBin, bgRejErr);
    m_NonTRRejVsTREff->SetBinContent(iBin, bgRej);
    m_NonTRRejVsTREff->SetBinError(iBin, bgRejErr);
  }
  m_NonTRRejVsTREffWidget->GetCanvas()->Modified();
  m_NonTRRejVsTREffWidget->GetCanvas()->Update();
}

void TRDLikelihoodPlot::finalize()
{
  m_NonTRHisto->Sumw2();
  if (m_NonTRHisto->GetEntries())
    m_NonTRHisto->Scale(1./m_NonTRHisto->GetEntries());
  m_TRHisto->Sumw2();
  if (m_TRHisto->GetEntries())
    m_TRHisto->Scale(1./m_TRHisto->GetEntries());

  double yMax = qMax(histogram(0)->GetMaximum(), histogram(1)->GetMaximum()) *1.05;
  histogram(0)->GetYaxis()->SetRangeUser(0, yMax);

  update();
}

bool TRDLikelihoodPlot::truthMcIsElectron(const AnalyzedEvent* analyzedEvent, bool& ok)
{
  const SimpleEvent* event = analyzedEvent->simpleEvent();
  if (event->contentType() == SimpleEvent::MonteCarlo) {
    ok = true;
    return qAbs(event->MCInformation()->primary()->pdgID) == 11;
  }
  if (!ok)
    return false;

  double c1Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV1);
  double c2Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV2);

  const Settings* settings = analyzedEvent->settings();
  Q_ASSERT(settings);
  bool eAboveCherenkov = settings->isAboveThreshold(Constants::electronMass);

  if (eAboveCherenkov && c1Signal > 200 && c2Signal > 200) {
    ok = true;
    return true;
  } else if (eAboveCherenkov && c1Signal < 100 && c2Signal < 100) {
    ok = true;
    return false;
  } else {
    ok = false;
    return false;
  }

}

int TRDLikelihoodPlot::binEfficiency(double effWanted, const TH1D* hist, double& eff, double& effErr)
{
  double nBins = hist->GetNbinsX();
  for (int i = 1; i < nBins+1; ++i) {
    eff = efficiency(hist, i, effErr);
    if (eff >= effWanted)
      return i;
  }
  return nBins;
}

double TRDLikelihoodPlot::efficiency(const TH1D* hist, int upToBin, double& effErr)
{
  double sigmaNAllEntries = 0;
  double sigmaNUpToBin = 0;
  double nAllEntries = hist->IntegralAndError(1,hist->GetNbinsX(), sigmaNAllEntries);
  if (qIsNull(nAllEntries)) {
    effErr = 0;
    return 0;
  }
  double nUpToBin = hist->IntegralAndError(1, upToBin, sigmaNUpToBin);
  double eff = nUpToBin / (nAllEntries);

  effErr = sqrt( pow(sigmaNUpToBin / nAllEntries, 2.) + pow(nUpToBin/(nAllEntries*nAllEntries)* sigmaNAllEntries, 2.));
  return eff;
}

double TRDLikelihoodPlot::rejection(const TH1D* hist, int upToBin, double& rejErr)
{
  double sigmaNAllEntries = 0;
  double sigmaNUpToBin = 0;
  double nAllEntries = hist->IntegralAndError(1,hist->GetNbinsX(), sigmaNAllEntries);
  double nUpToBin = hist->IntegralAndError(1, upToBin, sigmaNUpToBin);
  if (qIsNull(nUpToBin)) {
    rejErr = 0;
    return 0;
  }
  double rej = nAllEntries / nUpToBin;

  rejErr = sqrt( pow(sigmaNAllEntries / nUpToBin, 2.) + pow(nAllEntries/(nUpToBin*nUpToBin)* sigmaNUpToBin, 2.));
  return rej;
}

void TRDLikelihoodPlot::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  m_line50->Draw("same");
  m_line90->Draw("same");

  TVirtualPad* prevPad = gPad;
  TCanvas* can = m_NonTRRejVsTREffWidget->GetCanvas();
  can->cd();
  can->Clear();
  can->SetLogy(1);
  m_NonTRRejVsTREff->Draw();
  can->Modified();
  can->Update();
  prevPad->cd();
}

#include "TRDLikelihoodPlot.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TLine.h>

#include <math.h>

#include "TRDElectronIdentifierLikelihood.hh"
#include "SimpleEvent.hh"
#include "Constants.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "RootQtWidget.hh"

TRDLikelihoodPlot::TRDLikelihoodPlot(Topic topic)
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
  setTitle("-log(L) distribution");

  m_NonTRHisto = new TH1D(qPrintable(title() + "_nonTR"), "", 500, 0, 2);
  m_TRHisto = new TH1D(qPrintable(title() + "_TR"), "", 500, 0, 2);
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

void TRDLikelihoodPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{
  bool ok = false;
  bool isTRParticle = truthMcIsElectron(event, ok);
  if (!ok)
    return;

  double lhTR = 0.;
  bool validEvent = false;
  //bool identifiedAsTRParticle =
  m_EIdentifierLH->isElectronish(hits, particle, event, validEvent, lhTR);

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
  int iBineEff50 = getBinEff(0.5, m_TRHisto, eEff50, eEff50Err);
  double pRej50Err = 0;
  double pRej50 = getRej(m_NonTRHisto, iBineEff50, pRej50Err);

  double eEff90 = 0;
  double eEff90Err = 0;
  int iBineEff90 = getBinEff(0.9, m_TRHisto, eEff90, eEff90Err);
  double pRej90Err = 0;
  double pRej90 = getRej(m_NonTRHisto, iBineEff90, pRej90Err);

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
    int eEffBin = getBinEff(eEffWanted, m_TRHisto, eEff, eEffErr);
    double bgRejErr = 0;
    double bgRej = getRej(m_NonTRHisto, eEffBin, bgRejErr);
    m_NonTRRejVsTREff->SetBinContent(iBin, bgRej);
    m_NonTRRejVsTREff->SetBinError(iBin, bgRejErr);
  }
  m_NonTRRejVsTREffWidget->GetCanvas()->Modified();
  m_NonTRRejVsTREffWidget->GetCanvas()->Update();
}

void TRDLikelihoodPlot::finalize()
{
  m_NonTRHisto->Sumw2();
  m_NonTRHisto->Scale(1./m_NonTRHisto->GetEntries());
  m_TRHisto->Sumw2();
  m_TRHisto->Scale(1./m_TRHisto->GetEntries());

  double yMax = qMax(histogram(0)->GetMaximum(), histogram(1)->GetMaximum()) *1.05;
  histogram(0)->GetYaxis()->SetRangeUser(0, yMax);

  update();
}

bool TRDLikelihoodPlot::truthMcIsElectron(SimpleEvent* event, bool& ok)
{
  if (event->contentType() == SimpleEvent::MonteCarlo) {
    ok = true;
    return qAbs(event->MCInformation()->primary()->pdgID) == 11;
  }

  double c1Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV1);
  double c2Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV2);

  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
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

int TRDLikelihoodPlot::getBinEff(double effWanted, const TH1D* hist, double& eff, double& effErr)
{
  double nBins = hist->GetNbinsX();
  for (int i = 1; i < nBins+1; ++i) {
    eff = getEff(hist, i, effErr);
    if (eff >= effWanted)
      return i;
  }
  return nBins;
}

double TRDLikelihoodPlot::getEff(const TH1D* hist, int upToBin, double& effErr)
{
  double sigmaNAllEntries = 0;
  double sigmaNUpToBin = 0;
  double nAllEntries = hist->IntegralAndError(1,hist->GetNbinsX(), sigmaNAllEntries);
  double nUpToBin = hist->IntegralAndError(1, upToBin, sigmaNUpToBin);
  double eff = nUpToBin / (nAllEntries);

  effErr = sqrt( pow(sigmaNUpToBin / nAllEntries, 2.) + pow(nUpToBin/(nAllEntries*nAllEntries)* sigmaNAllEntries, 2.));
  return eff;
}

double TRDLikelihoodPlot::getRej(const TH1D* hist, int upToBin, double& rejErr)
{
  double sigmaNAllEntries = 0;
  double sigmaNUpToBin = 0;
  double nAllEntries = hist->IntegralAndError(1,hist->GetNbinsX(), sigmaNAllEntries);
  double nUpToBin = hist->IntegralAndError(1, upToBin, sigmaNUpToBin);
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

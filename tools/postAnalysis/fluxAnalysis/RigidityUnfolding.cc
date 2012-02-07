#include "RigidityUnfolding.hh"

#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>
#include <TArray.h>
#include <TSpline.h>
#include <TArrayD.h>
#include <TUnfold.h>
#include <TGraph.h>

#include <cmath>
#include <iostream>
#include <iomanip>

#include <QDebug>

RigidityUnfolding::RigidityUnfolding(TH2D* migrationHistogram, TH1D* unfoldInput, double tau)
  : m_migrationHistogram(new TH2D(*migrationHistogram))
  , m_unfoldInput(new TH1D(*unfoldInput))
  , m_tauOverride(false)
  , m_unfoldedHistogram(0)
  , m_rhoIj(0)
  , m_lCurve(0)
{
  if (!qFuzzyCompare(tau, 9999.))
    m_tauOverride = true;
  m_unfoldInput->SetBinContent(0, 0);
  m_unfoldInput->SetBinContent(m_unfoldInput->GetNbinsX() + 1, 0);
  unfold(tau);
}

RigidityUnfolding::~RigidityUnfolding()
{
  delete m_migrationHistogram;
  delete m_unfoldInput;
  delete m_unfoldedHistogram;
  delete m_lCurve;
}

void RigidityUnfolding::unfold(double tau)
{
  TUnfold unfold(m_migrationHistogram, TUnfold::kHistMapOutputVert);
  const int nGen = m_migrationHistogram->GetNbinsY();
  QVector<double> binning(nGen + 1);
  for (int bin = 0; bin <= nGen; ++bin)
    binning[bin] = m_migrationHistogram->GetYaxis()->GetBinLowEdge(bin + 1);

  if (unfold.SetInput(m_unfoldInput) >= 10000)
    qDebug() << "Unfolding result may be wrong";

  int nScan = 100;//70
  double tauMin = 1e-7;//1e-7; // to use automatic L-curve scan start with taumin = taumax = 0.0
  double tauMax = 1;//1.;
  int iBest;
  TSpline *logTauX = 0, *logTauY = 0;
  TGraph* lCurve = 0;

  iBest = unfold.ScanLcurve(nScan, tauMin, tauMax, &lCurve, &logTauX, &logTauY);
  m_lCurve = new TGraph(*lCurve);
  m_lCurve->GetXaxis()->SetTitle("log_{10}(#Chi^{2})");
  m_lCurve->GetYaxis()->SetTitle("log_{10}(regularisation condition)");

  double t = 0, x = 0, y = 0;
  logTauX->GetKnot(iBest, t, x);
  logTauY->GetKnot(iBest, t, y);
  if (m_tauOverride) {
    unfold.DoUnfold(tau);
    x = log10(tau);
    y = m_lCurve->Eval(x);
  }
  TGraph* bestLcurve = new TGraph(1, &x, &y);

  m_bestLcurve = new TGraph(*bestLcurve);
  m_bestLcurve->SetMarkerColor(kRed);

  qDebug() << "tau =" << unfold.GetTau();
  qDebug() << "chi2 =" << unfold.GetChi2A() << "+" << unfold.GetChi2L() << "/" << unfold.GetNdf();

  int* binMap = new int[nGen + 2];
  for (int i = 1; i <= nGen; ++i)
    binMap[i] = i;
  binMap[0] = -1; // discarde underflow bin
  binMap[nGen+1] = -1; // discarde overflow bin

  const QString& title = "unfolded";
  const QString& xTitle = m_unfoldInput->GetXaxis()->GetTitle();

  m_unfoldedHistogram = new TH1D(qPrintable(title), qPrintable(xTitle), nGen, binning.constData());
  m_unfoldedHistogram->Sumw2();
  m_unfoldedHistogram->SetMarkerColor(kRed);
  m_unfoldedHistogram->SetLineColor(kRed);

  unfold.GetOutput(m_unfoldedHistogram, binMap);

  m_rhoIj = unfold.GetRhoIJ("", "correlation");

  delete[] binMap;
  binMap = 0;
}

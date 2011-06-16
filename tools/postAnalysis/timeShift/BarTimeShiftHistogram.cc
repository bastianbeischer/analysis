#include "BarTimeShiftHistogram.hh"
#include "RootStyle.hh"
#include "BarShiftPlot.hh"
#include "Constants.hh"
#include "TimeShiftContainer.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TStyle.h>

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QDebug>
#include <QStringList>

BarTimeShiftHistogram::BarTimeShiftHistogram(const QVector<BarShiftPlot*>& plots)
  : PostAnalysisPlot()
  , H2DPlot()
{
  setTitle("bar time shift");

  TH2D* histogram = new TH2D("barShiftHistogram", ";;;#Deltat / ns", 4, 0, 4, 4, 0, 4);

  foreach (BarShiftPlot* plot, plots) {
    int xBin = 0;
    int yBin = 0;
    if (plot->title().contains("bar shift 0x8000 0x8010")) xBin = 1;
    if (plot->title().contains("bar shift 0x8004 0x8014")) xBin = 2;
    if (plot->title().contains("bar shift 0x8008 0x8018")) xBin = 3;
    if (plot->title().contains("bar shift 0x800c 0x801c")) xBin = 4;
    if (plot->title().contains("0x8020 0x8030")) yBin = 1;
    if (plot->title().contains("0x8024 0x8034")) yBin = 2;
    if (plot->title().contains("0x8028 0x8038")) yBin = 3;
    if (plot->title().contains("0x802c 0x803c")) yBin = 4;
    histogram->SetBinContent(xBin, yBin, plot->dt() - desiredTimeDifference(qAbs(xBin-yBin)));
  }
  histogram->GetXaxis()->SetBinLabel(1, "8000 8010");
  histogram->GetXaxis()->SetBinLabel(2, "8004 8014");
  histogram->GetXaxis()->SetBinLabel(3, "8008 8018");
  histogram->GetXaxis()->SetBinLabel(4, "800c 801c");
  histogram->GetYaxis()->SetBinLabel(1, "8020 8030");
  histogram->GetYaxis()->SetBinLabel(2, "8024 8034");
  histogram->GetYaxis()->SetBinLabel(3, "8028 8038");
  histogram->GetYaxis()->SetBinLabel(4, "802c 803c");
  histogram->GetZaxis()->SetRangeUser(-0.8, 0.8);
  histogram->SetMarkerColor(kRed);
  setPalette(RootStyle::ResiduePalette);
  setDrawOption(COLZTEXT);
  addHistogram(histogram);

  align(plots);
}

BarTimeShiftHistogram::~BarTimeShiftHistogram()
{}

double BarTimeShiftHistogram::desiredTimeDifference(int barPositionDistance)
{
  double z = (Constants::upperTofPosition - Constants::lowerTofPosition);
  double x = barPositionDistance * Constants::tofBarWidth;
  return sqrt(x*x + z*z) / Constants::speedOfLight;
}

void BarTimeShiftHistogram::align(const QVector<BarShiftPlot*>& plots)
{
  QVector<int> upperBar;
  QVector<int> lowerBar;
  QVector<double> bVector;
  QVector<double> errBVector;

  foreach(BarShiftPlot* plot, plots) {
    if (true || !isnan(plot->dt())) {
      int upper = 0;
      int lower = 0;
      if (plot->title().contains("bar shift 0x8000 0x8010")) upper = 0;
      if (plot->title().contains("bar shift 0x8004 0x8014")) upper = 1;
      if (plot->title().contains("bar shift 0x8008 0x8018")) upper = 2;
      if (plot->title().contains("bar shift 0x800c 0x801c")) upper = 3;
      if (plot->title().contains("0x8020 0x8030")) lower = 0;
      if (plot->title().contains("0x8024 0x8034")) lower = 1;
      if (plot->title().contains("0x8028 0x8038")) lower = 2;
      if (plot->title().contains("0x802c 0x803c")) lower = 3;
      int barPositionDistance = qAbs(upper - lower);
      upperBar.append(upper);
      lowerBar.append(lower);
      bVector.append(desiredTimeDifference(barPositionDistance) - plot->dt());
      errBVector.append(plot->errDt());
    }
  }

  int n = bVector.count();
  TMatrixT<double> b(n, 1);
  for (int i = 0; i < n; ++i) {
    b[i][0] = bVector[i];
  }

  int m = Constants::nTofBars/2;
  TMatrixT<double> AFull(n, m);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      AFull[i][j] = 0;
    }
    int j = 0;
    j = upperBar[i];
    AFull[i][j] = -1;
    j = lowerBar[i] + 4;
    AFull[i][j] = 1;
  }
  TMatrixT<double> A;
  AFull.GetSub(0, n-1, 1, m-1, A);
  --m;
  
  TMatrixT<double> At(m, n);
  At.Transpose(A);
  TMatrixT<double> Result = (At * A).Invert() * At * b;
  //dumpMatrix(A);
  //dumpMatrix(b);
  //dumpMatrix(Result);
  double result[Constants::nTofBars/2];
  result[0] = 0;
  for (int i = 0; i < Constants::nTofBars/2 - 1; ++i)
    result[i+1] = Result[i][0];
  TimeShiftContainer::instance()->applyBarShifts(result);
}

void BarTimeShiftHistogram::dumpMatrix(const TMatrixT<double>& m)
{
  int width = 9;
  int space = 2;
  std::cout << " _ ";
  for (int i = 0; i < (width + space) * m.GetNcols() - space + 1; ++i) std::cout << ' ';
  std::cout << "_ " << std::endl;
  for (int r = 0; r < m.GetNrows(); ++r) {
    for (int c = 0; c < m.GetNcols(); ++c) {
      if (c) {
        for (int i = 0; i < space; ++i) std::cout << ' ';
      } else {
        std::cout << (r == m.GetNrows() - 1 ? "|_ " : "|  ");
      }
      std::cout << std::setfill('x') << std::setw(width) << std::showpos << std::fixed << m[r][c];
    }
    std::cout << (r == m.GetNrows() - 1 ? " _|" : "  |") << std::endl;
  }
}

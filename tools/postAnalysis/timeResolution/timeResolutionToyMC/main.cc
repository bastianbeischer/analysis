#include "Constants.hh"
#include "Helpers.hh"
#include "RootStyle.hh"

#include <TRandom3.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TApplication.h>

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <QDebug>

double resolutionDistribution(double* x, double* p)
{
  return p[0] + p[1] * pow(2. * (x[0]-p[2]) / Constants::tofBarLength, 2);
}

int main(int argc, char* argv[])
{
  RootStyle::set();
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " numberOfBinsAlongY outputFile.root" << std::endl;
    return -1;
  }
  int nBins = atoi(argv[1]);
  double binWidth = Constants::tofBarLength / nBins;
  char* outputFileName = strdup(argv[2]);

  TApplication application("timeResolutionToyMC", &argc, argv);

  TF1* resolutionFunction[4][2];
  TH1D* resolutionHistogram[4][2];
  for (int bar = 0; bar < 4; ++bar) {
    for (int layer = 0; layer < 2; ++layer) {
      char name[128];
      sprintf(name, "resolutionFunction_%d_%d", bar, layer);
      resolutionFunction[bar][layer] = new TF1(name, resolutionDistribution, -Constants::tofBarLength / 2., Constants::tofBarLength / 2., 3);
      resolutionFunction[bar][layer]->SetParameter(0, 0.05 * bar + 0.3 / sqrt(2.));
      resolutionFunction[bar][layer]->SetParameter(1, 0.1 * (bar + 1));
      resolutionFunction[bar][layer]->SetParameter(2, (0.5 * layer + bar - 2.5) * 20);
      sprintf(name, "resolutionHistogram_%d_%d", bar, layer);
      resolutionHistogram[bar][layer] = new TH1D(name, ";y / mm;#sigma / ns", nBins, -Constants::tofBarLength / 2., Constants::tofBarLength / 2.);
      for (int bin = 1; bin <= nBins; ++bin) {
        double y = resolutionHistogram[bar][layer]->GetXaxis()->GetBinCenter(bin);
        resolutionHistogram[bar][layer]->SetBinContent(bin, resolutionFunction[bar][layer]->Eval(y));
      }
    }
  }

  TH2D* histograms[4][4];
  histograms[0][0] = new TH2D("time resolution 0x8000 0x8010, 0x8020 0x8030", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[0][1] = new TH2D("time resolution 0x8000 0x8010, 0x8024 0x8034", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[0][2] = new TH2D("time resolution 0x8000 0x8010, 0x8028 0x8038", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[0][3] = new TH2D("time resolution 0x8000 0x8010, 0x802c 0x803c", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[1][0] = new TH2D("time resolution 0x8004 0x8014, 0x8020 0x8030", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[1][1] = new TH2D("time resolution 0x8004 0x8014, 0x8024 0x8034", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[1][2] = new TH2D("time resolution 0x8004 0x8014, 0x8028 0x8038", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[1][3] = new TH2D("time resolution 0x8004 0x8014, 0x802c 0x803c", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[2][0] = new TH2D("time resolution 0x8008 0x8018, 0x8020 0x8030", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[2][1] = new TH2D("time resolution 0x8008 0x8018, 0x8024 0x8034", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[2][2] = new TH2D("time resolution 0x8008 0x8018, 0x8028 0x8038", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[2][3] = new TH2D("time resolution 0x8008 0x8018, 0x802c 0x803c", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[3][0] = new TH2D("time resolution 0x800c 0x801c, 0x8020 0x8030", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[3][1] = new TH2D("time resolution 0x800c 0x801c, 0x8024 0x8034", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[3][2] = new TH2D("time resolution 0x800c 0x801c, 0x8028 0x8038", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  histograms[3][3] = new TH2D("time resolution 0x800c 0x801c, 0x802c 0x803c", ";;#Deltat / ns;", nBins*nBins, 0., nBins*nBins, 30, 0., 6.);
  
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      for (int upperBin = 0; upperBin < nBins; ++upperBin) {
        double upperCenter = (0.5 + upperBin) * binWidth - Constants::tofBarLength / 2.;
        for (int lowerBin = 0; lowerBin < nBins; ++lowerBin) {
          double lowerCenter = (0.5 + lowerBin) * binWidth - Constants::tofBarLength / 2.;
          QString label = QString("u%1 l%2").arg(upperCenter).arg(lowerCenter);
          histograms[i][j]->GetXaxis()->SetBinLabel(upperBin * nBins + lowerBin + 1, qPrintable(label));
        }
      }
    }
  }

  TRandom* random = new TRandom();
  for (int event = 0; event < 50000 * 4 * 4 * nBins * nBins; ++event) {
    double upperX = random->Uniform(- 2. * Constants::tofBarWidth, 2. * Constants::tofBarWidth);
    double lowerX = random->Uniform(- 2. * Constants::tofBarWidth, 2. * Constants::tofBarWidth);
    int upperBar = (upperX + 2. * Constants::tofBarWidth) / Constants::tofBarWidth;
    int lowerBar = (lowerX + 2. * Constants::tofBarWidth) / Constants::tofBarWidth;
    double upperY = random->Uniform(- Constants::tofBarLength / 2., Constants::tofBarLength / 2.);
    double lowerY = random->Uniform(- Constants::tofBarLength / 2., Constants::tofBarLength / 2.);
    int upperBin = (upperY + Constants::tofBarLength / 2.) / binWidth;
    int lowerBin = (lowerY + Constants::tofBarLength / 2.) / binWidth;
    double ds =
      Helpers::addQuad(upperX - lowerX, upperY - lowerY, Constants::upperTofPosition - Constants::lowerTofPosition);
    double upperResolution = resolutionHistogram[upperBar][0]->GetBinContent(upperBin + 1);
    double lowerResolution = resolutionHistogram[lowerBar][1]->GetBinContent(lowerBin + 1);
    double t1 = random->Gaus(0, upperResolution);
    double t2 = random->Gaus(ds / Constants::speedOfLight, lowerResolution);
    double dt = t2 - t1;
    histograms[upperBar][lowerBar]->Fill(upperBin * nBins + lowerBin, dt);
  }

  TCanvas* canvases[4][4];
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      char canvasName[128];
      sprintf(canvasName, "%s canvas", histograms[i][j]->GetName());
      canvases[i][j] = new TCanvas(canvasName, "");
      histograms[i][j]->Draw("COLZ");
    }
  }
  TCanvas* canvas = new TCanvas("timeResolutionToyMC canvas", "timeResolutionToyMC", 1);
  for (int bar = 0; bar < 4; ++bar) {
    resolutionHistogram[bar][0]->SetLineStyle(1);
    resolutionHistogram[bar][1]->SetLineStyle(2);
    resolutionHistogram[bar][0]->SetLineColor(1 + bar);
    resolutionHistogram[bar][1]->SetLineColor(1 + bar);
    resolutionHistogram[bar][0]->SetLineWidth(2);
    resolutionHistogram[bar][1]->SetLineStyle(2);
    if (bar == 0) {
      resolutionHistogram[bar][0]->Draw();
      resolutionHistogram[bar][0]->GetYaxis()->SetRangeUser(0., 1.);
    } else {
      resolutionHistogram[bar][0]->Draw("SAME");
    }
    resolutionHistogram[bar][1]->Draw("SAME");
  }

  TFile file(outputFileName, "RECREATE");
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      canvases[i][j]->Write();
  canvas->Write();
  file.Close();
  application.Run();
  return 0;
}

#include "TrackerTemperaturePlot.hh"
#include "PostAnalysisCanvas.hh"
#include "TriggerRateTimePlot.hh"
#include "TrackerTemperaturePlot.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TVector3.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

TrackerTemperaturePlot::TrackerTemperaturePlot(const QVector<PostAnalysisCanvas*> canvases)
  : PostAnalysisPlot()
  , H1DPlot()
{
  setTitle("tracker temperature plot");
  foreach(PostAnalysisCanvas* canvas, canvases) {
    TH2D* h = canvas->histograms2D().at(0);
    int nBinsX = h->GetXaxis()->GetNbins();
    double minX = h->GetXaxis()->GetXmin();
    double maxX = h->GetXaxis()->GetXmax();
    TH1D* histogram = new TH1D(qPrintable(title() + canvas->name()), "", nBinsX, minX, maxX);
    for (int binX = 1; binX <= nBinsX; ++binX) {
      TH1D* projection = h->ProjectionY(qPrintable(title() + "projection"), binX, binX);
      histogram->SetBinContent(binX, projection->GetMean());
      delete projection;
    }
    histogram->GetXaxis()->SetTimeDisplay(1);
    histogram->GetXaxis()->SetTimeFormat("%d-%H:%M");
    histogram->GetXaxis()->SetTitle("time");
    histogram->GetYaxis()->SetTitle("height / km");
    histogram->SetMarkerStyle(20);
    histogram->SetMarkerSize(0.2);
    int id = canvas->name().mid(6, 4).toInt(0, 16);
    const TVector3& position = Setup::instance()->element(id & 0xFFF0)->position();
    histogram->SetMarkerColor(layer(position.z()));
    //qDebug() << hex << id << "->" << layer(position.z()) << position.x() << position.y() << position.z();
    addHistogram(histogram);
  }
}

TrackerTemperaturePlot::~TrackerTemperaturePlot()
{}

int TrackerTemperaturePlot::layer(double z)
{
  if (z > 200.) {
    return 1;
  } else if (z > 0.) {
    return 2;
  } else if (z > -200) {
    return 3;
  } else {
    return 4;
  }
}

void TrackerTemperaturePlot::draw(TCanvas* canvas)
{
  canvas->cd();
  histogram(0)->Draw("P");
  for (int i = 1; i < numberOfHistograms(); ++i)
    histogram(i)->Draw("SAME P");
  gPad->Modified();
  gPad->Update();
}

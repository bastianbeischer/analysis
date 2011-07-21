#include "TrackerTemperaturePlot.hh"
#include "PostAnalysisCanvas.hh"
#include "TrackerTemperaturePlot.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

#include <TGraph.h>
#include <TMultiGraph.h>
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
  , GraphPlot()
{
  setTitle("tracker temperature plot");
  foreach(PostAnalysisCanvas* canvas, canvases) {
    TH1D* h0 = canvas->histograms1D().at(0);
    int nBins = h0->GetXaxis()->GetNbins();
    TGraph* g = new TGraph;
    for (int bin = 1; bin <= nBins; ++bin) {
      double temperature = h0->GetBinContent(bin);
      if (!qFuzzyCompare(temperature, 0))
        g->SetPoint(g->GetN(), h0->GetXaxis()->GetBinCenter(bin), temperature);
    }
    int id = canvas->name().mid(6, 4).toInt(0, 16);
    const TVector3& position = Setup::instance()->element(id & 0xFFF0)->position();
    g->SetMarkerColor(layer(position.z()));
    g->SetMarkerStyle(20);
    g->SetMarkerSize(0.2);
    addGraph(g, P);
    //qDebug() << hex << id << "->" << layer(position.z()) << position.x() << position.y() << position.z();
  }
  xAxis()->SetTimeDisplay(1);
  xAxis()->SetTimeFormat("%d-%H:%M");
  setAxisTitle("time", "#vartheta / #circC");
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

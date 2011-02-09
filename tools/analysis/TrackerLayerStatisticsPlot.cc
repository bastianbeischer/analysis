#include "TrackerLayerStatisticsPlot.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TLegend.h>

#include "Track.hh"
#include "TrackInformation.hh"

TrackerLayerStatisticsPlot::TrackerLayerStatisticsPlot() :
  AnalysisPlot(AnalysisPlot::MiscellaneousTracker),
  H1DPlot()
{
  setTitle("tracker layer statistics");
  TH1D* histogram = new TH1D(qPrintable(title()+"_1"), "", 9, -0.5, 8.5);
  histogram->GetXaxis()->SetNdivisions(9);
  histogram->GetXaxis()->SetTitle("number of layers");
  histogram->GetYaxis()->SetTitle("relative entries");
  addHistogram(histogram);
  histogram = new TH1D(qPrintable(title()+"_2"), "", 9, -0.5, 8.5);
  histogram->SetLineColor(kRed);
  histogram->SetFillColor(kRed);
  histogram->SetFillStyle(3004);
  addHistogram(histogram);
  histogram = new TH1D(qPrintable(title()+"_3"), "", 9, -0.5, 8.5);
  histogram->SetLineColor(kBlue);
  histogram->SetFillColor(kBlue);
  histogram->SetFillStyle(1001);
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(0.42, 0.5));

  TLegend* legend = new TLegend(0.2, 0.5, 0.4, 0.7);
  legend->AddEntry(this->histogram(0),"layers in event >= x", "lf");
  legend->AddEntry(this->histogram(1),"layers in event == x", "lf");
  legend->AddEntry(this->histogram(2),"layers in event == x && hits == x", "lf");
  addLegend(legend);
}

TrackerLayerStatisticsPlot::~TrackerLayerStatisticsPlot()
{
}

void TrackerLayerStatisticsPlot::processEvent(const QVector<Hit*>& /*hits*/, Track* track, SimpleEvent*)
{
  if (!track || !track->fitGood())
    return;

  const TrackInformation* info = track->information();
  QMap<double, int> hitsInLayers = info->hitsInLayers();
  unsigned short nLayers = info->numberOfTrackerLayers();

  histogram(0)->Fill(0);
  for (int i = 0; i < nLayers; i++) {
    histogram(0)->Fill(i+1);
  }
  histogram(1)->Fill(nLayers);

  foreach(int count, hitsInLayers) {
    if (count != 1)
      return;
  }
  histogram(2)->Fill(nLayers);
}

void TrackerLayerStatisticsPlot::update()
{
  double content7 = histogram(2)->GetBinContent(8);
  double content8 = histogram(2)->GetBinContent(9);
  double ratio = content8 / (content7+content8);
  latex(0)->SetTitle(qPrintable(QString("ratio 8/(7+8) = %1").arg(ratio,0,'f',2)));
}

void TrackerLayerStatisticsPlot::finalize()
{
  double factor = 1./histogram()->GetBinContent(1);
  histogram(0)->Scale(factor);
  histogram(1)->Scale(factor);
  histogram(2)->Scale(factor);
  histogram(0)->GetYaxis()->SetRangeUser(0., 1.05);
}
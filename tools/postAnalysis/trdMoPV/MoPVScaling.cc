#include "MoPVScaling.hh"

#include "PostAnalysisCanvas.hh"

#include <TGraphErrors.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TLatex.h>

#include <QDebug>

#include <math.h>

MoPVScaling::MoPVScaling(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , GraphPlot()
{
  TH2D* histogram = canvas->histograms2D().at(0);
  m_mopvGraph = generateMoPVGraph(histogram);

  QString title = QString(canvas->name()).replace("canvas", "graph");
  setTitle(title);
  addGraph(m_mopvGraph, "P");

  setAxisTitle("time", "trd mopv");
  
}

MoPVScaling::~MoPVScaling()
{
}

TGraphErrors* MoPVScaling::generateMoPVGraph(TH2D* histogram)
{

  QVector<double> x, y, xErr, yErr;

  for (int bin = 1; bin < histogram->GetNbinsX()+1; ++bin) {
    TH1* projectionHistogram = histogram->ProjectionY("_py", bin, bin);
    int nEntries = projectionHistogram->GetEntries();
    if (nEntries < 30)
        continue;
    //which fit range ?!?, use same as TRD spectrum ?!?
    TF1* function = new TF1(qPrintable(QString(histogram->GetTitle()) + "Function"), "landau", histogram->GetYaxis()->GetXmin(), histogram->GetYaxis()->GetXmax());
    projectionHistogram->Fit(function, "EQN0");

    double mpv = function->GetParameter(1);
    double mpvErr = function->GetParError(1);

    delete projectionHistogram;
    delete function;

    double time = histogram->GetXaxis()->GetBinCenter(bin);
    double timeErr = histogram->GetXaxis()->GetBinWidth(bin)/sqrt(12);
    
    x << time;
    xErr << timeErr;
    y << mpv;
    yErr << mpvErr;
  }
  
  return new TGraphErrors(x.size(), &x[0], &y[0], &xErr[0], &yErr[0]);

}

#include "MoPVScaling.hh"

#include "PostAnalysisCanvas.hh"

#include <TGraphErrors.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TLatex.h>

#include <QList>
#include <QRegExp>

#include <math.h>

MoPVScaling::MoPVScaling(PostAnalysisCanvas* canvas)
  : QObject()
  , PostAnalysisPlot()
  , GraphPlot()
  , m_colorCounter(0)
  , m_dependencyFit(0)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  TGraph* graph = generateMoPVGraph(histogram);
  m_dependencyFit = new TF1("dependencyFit", "pol1");
  graph->Fit(m_dependencyFit);
  QString title = QString(canvas->name()).replace("canvas", "graph");
  setTitle(title);
  addGraph(graph, "L");
  setAxisTitle(histogram->GetXaxis()->GetTitle(), "trd mopv");
}

MoPVScaling::MoPVScaling(QList<PostAnalysisCanvas*> canvasList)
  : QObject()
  , PostAnalysisPlot()
  , GraphPlot()
  , m_colorCounter(0)
  , m_dependencyFit(0)
{
  //TODO is first name ok?
  QString title = QString(canvasList.first()->name()).replace("canvas", "graph");
  setTitle(title.replace(QRegExp("\\dx\\d\\d\\d."),""));
  setAxisTitle(canvasList.first()->histograms2D().at(0)->GetXaxis()->GetTitle(), "trd mopv");

  foreach (PostAnalysisCanvas* canvas, canvasList) {
    TH2D* histogram = canvas->histograms2D().at(0);
    TGraph* graph = generateMoPVGraph(histogram);
    if (!graph)
      continue;
    graph->SetLineColor(RootStyle::rootColor(m_colorCounter++));
    graph->SetLineWidth(1);
    graph->SetDrawOption("LX");
    addGraph(graph, "LX");
  }
  drawOption(ALX);
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
    if (nEntries < 100)
        continue;
    //which fit range ?!?, use same as TRD spectrum ?!?
    TF1* functionLandau = new TF1(qPrintable(QString(histogram->GetTitle()) + "FunctionLandau"), "landau", 0.1, 3);
    projectionHistogram->Fit(functionLandau, "EQN0");

    double mpv = functionLandau->GetParameter(1);
    double mpvErr = functionLandau->GetParError(1);

    //fit again:
    /*
    projectionHistogram->Fit(functionLandau, "EQN0R", 0, mpv-0.5*mpv, mpv+1.5*mpv);
    mpv = functionLandau->GetParameter(1);
    mpvErr = functionLandau->GetParError(1)

    TF1* functionGaus = new TF1(qPrintable(QString(histogram->GetTitle()) + "FunctionGaus"), "gaus", mpv-0.2*mpv, mpv+0.2*mpv);
    projectionHistogram->Fit(functionGaus, "EQN0");
    double mpvGaus = functionGaus->GetParameter(1);
    double mpvGausErr = functionGaus->GetParError(1);
    delete functionGaus;
    */

    delete projectionHistogram;
    delete functionLandau;


    double xValue = histogram->GetXaxis()->GetBinCenter(bin);
    double xValueErr = 0; //histogram->GetXaxis()->GetBinWidth(bin)/sqrt(12);
    
    x << xValue;
    xErr << xValueErr;
    y << mpv;
    yErr << mpvErr;
  }

  if (x.size() == 0)
    return 0;
  return new TGraphErrors(x.size(), &x[0], &y[0], &xErr[0], &yErr[0]);
}

void MoPVScaling::saveDependency()
{
}

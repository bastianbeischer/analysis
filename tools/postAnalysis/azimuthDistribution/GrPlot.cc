#include "GrPlot.hh"

#include "Corrections.hh"

#include <TH1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TLatex.h>

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QDebug>
#include <QStringList>
#include <QSettings>
#include <QVector>

GrPlot::GrPlot(TGraph* graph)
: PostAnalysisPlot()
, GraphPlot()
{
  setTitle(graph->GetTitle());
  addGraph(new TGraph(*graph));
  setAxisTitle(graph->GetXaxis()->GetTitle(), graph->GetYaxis()->GetTitle());
}

GrPlot::GrPlot(TGraph* graph1, TGraph* graph2)
: PostAnalysisPlot()
, GraphPlot()
{
  setTitle(graph1->GetTitle());
  addGraph(new TGraph(*graph1));
  setAxisTitle(graph1->GetXaxis()->GetTitle(), graph1->GetYaxis()->GetTitle());
  addGraph(graph2);
}

GrPlot::~GrPlot()
{

}

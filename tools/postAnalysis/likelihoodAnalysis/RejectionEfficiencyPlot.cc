#include "RejectionEfficiencyPlot.hh"
#include "LogLikelihoodProjectionPlot.hh"

#include <TGraph.h>
#include <TAxis.h>
#include <TH1.h>
#include <TLine.h>

#include <QDebug>

RejectionEfficiencyPlot::RejectionEfficiencyPlot(LogLikelihoodProjectionPlot* projection)
  : PostAnalysisPlot()
  , GraphPlot()
  , m_verticalLines()
  , m_horizontalLines()
{
  QString title = projection->title();
  title.replace("log likelihood histogram", "rejection efficiency");
  setTitle(title);

  TAxis* xAxis = projection->signalProjection()->GetXaxis();
  int nBins = xAxis->GetNbins();
  Q_ASSERT(nBins == projection->backgroundProjection()->GetXaxis()->GetNbins());
  const double* signal = projection->signalProjection()->GetIntegral();
  const double* background = projection->backgroundProjection()->GetIntegral();

  TGraph* graph = new TGraph(nBins);
  graph->SetName(qPrintable(title));
  for (int bin = 1; bin <= nBins; ++bin) {
    double efficiency = signal[bin];
    double rejection = qIsNull(background[bin]) ? 1e9 : 1. + signal[bin]/background[bin];
    graph->SetPoint(bin-1, efficiency, rejection);
  }
  addGraph(graph, RootPlot::AP);
  setAxisTitle("efficiency", "rejection");

  QVector<double> efficiencies = QVector<double>() << 0.5 << 0.9;
  foreach (double efficiency, efficiencies) {
    double rejection = graph->Eval(efficiency);
    TLine* line = 0;
    line = new TLine(efficiency, .0, efficiency, rejection);
    line->SetLineStyle(kDotted);
    line->SetLineWidth(2);
    line->SetLineColor(kBlue);
    line->SetVertical();
    m_verticalLines.append(line);
    line = new TLine(.0, rejection, efficiency, rejection);
    line->SetLineStyle(kDotted);
    line->SetLineWidth(2);
    line->SetLineColor(kBlue);
    line->SetHorizontal();
    m_horizontalLines.append(line);
  }
}

RejectionEfficiencyPlot::~RejectionEfficiencyPlot()
{
  qDeleteAll(m_verticalLines);
  qDeleteAll(m_horizontalLines);
}

void RejectionEfficiencyPlot::draw(TCanvas* canvas)
{
  GraphPlot::draw(canvas);
  double xMin = graph()->GetXaxis()->GetXmin();
  double yMin = graph()->GetYaxis()->GetXmin();

  foreach (TLine* line, m_verticalLines) {
    line->SetY1(yMin);
    line->Draw();
  }
  foreach (TLine* line, m_horizontalLines) {
    line->SetX1(xMin);
    line->Draw();
  }
}

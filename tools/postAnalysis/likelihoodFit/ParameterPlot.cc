#include "ParameterPlot.hh"
#include "Likelihood.hh"
#include "LikelihoodPDF.hh"
#include "PDFParameters.hh"

#include <TGraph.h>
#include <TLine.h>
#include <TAxis.h>

#include <QDebug>

ParameterPlot::ParameterPlot(Likelihood* likelihood, Enums::Particle particle)
  : PostAnalysisPlot()
  , GraphPlot()
  , m_likelihood(likelihood)
  , m_particle(particle)
  , m_numberOfPoints(2*105+1)
  , m_min(0.)
  , m_max(10.5)
  , m_step((m_max - m_min) / (m_numberOfPoints - 1))
{
  setTitle(QString("%1 parameters %2").arg(m_likelihood->title()).arg(Enums::label(m_particle)));
  setAxisTitle("|R| / GV", "parameter / a.u.");
  setup();
}

ParameterPlot::~ParameterPlot()
{
}

void ParameterPlot::setup()
{
  removeGraphs();
  qDeleteAll(m_lines);
  m_lines.clear();

  double markerSize = 0.8;
  int numberOfParameters = m_likelihood->numberOfParameters();

  QVector<TGraph*> extrapolatedGraphs;
  QVector<TGraph*> interpolatedGraphs;
  QVector<TGraph*> nodeGraphs;
  for (int parameter = 0; parameter < numberOfParameters; ++parameter) {
    TGraph* g = 0;
    g = new TGraph;
    g->SetName(qPrintable(QString("parameter%1 extrapolated").arg(parameter)));
    g->SetMarkerSize(markerSize);
    g->SetMarkerColor(kRed);
    g->SetLineColor(kRed);
    extrapolatedGraphs.append(g);
    g = new TGraph;
    g->SetName(qPrintable(QString("parameter%1 interpolated").arg(parameter)));
    g->SetMarkerSize(markerSize);
    g->SetMarkerColor(kMagenta);
    g->SetLineColor(kMagenta);
    interpolatedGraphs.append(g);
    g = new TGraph();
    g->SetMarkerSize(markerSize);
    g->SetName(qPrintable(QString("parameter%1 node").arg(parameter)));
    g->SetMarkerColor(kBlack);
    g->SetLineWidth(2);
    g->SetLineColor(kBlack);
    nodeGraphs.append(g);
  }

  TGraph* normalizationFactorGraph = new TGraph(m_numberOfPoints);
  normalizationFactorGraph->SetName("normalizationFactorGraph");
  normalizationFactorGraph->SetMarkerSize(markerSize);
  normalizationFactorGraph->SetMarkerColor(kGreen);
  normalizationFactorGraph->SetLineColor(kGreen);

  const QVector<double> rigidities = m_likelihood->rigidityNodes(m_particle);
  if (numberOfParameters) {
    for (int point = 0; point < m_numberOfPoints; ++point) {
      double absoluteRigidity = m_min + point * m_step;
      bool goodInterpolation = false;
      PDFParameters parameters = m_likelihood->interpolation(Hypothesis(m_particle, 1. / absoluteRigidity), &goodInterpolation);
      Q_ASSERT(parameters.count() == numberOfParameters);
      for (int parameter = 0; parameter < numberOfParameters; ++parameter) {
        TGraph* g = goodInterpolation ? interpolatedGraphs[parameter] : extrapolatedGraphs[parameter];
        g->SetPoint(g->GetN(), absoluteRigidity, parameters[parameter]);
      }
      normalizationFactorGraph->SetPoint(point, absoluteRigidity, parameters.normalizationFactor());
    }
    foreach (double rigidity, rigidities) {
      PDFParameters parameters = m_likelihood->interpolation(Hypothesis(m_particle, 1. / rigidity));
      for (int parameter = 0; parameter < numberOfParameters; ++parameter) {
        TGraph* g = nodeGraphs[parameter];
        g->SetPoint(g->GetN(), rigidity, parameters[parameter]);
      }
    }
  }

  foreach (TGraph* g, extrapolatedGraphs)
    if (g->GetN())
      addGraph(g, RootPlot::P);
  foreach (TGraph* g, interpolatedGraphs)
    if (g->GetN())
      addGraph(g, RootPlot::P);
  foreach (TGraph* g, nodeGraphs)
    if (g->GetN())
      addGraph(g, RootPlot::LP);
  addGraph(normalizationFactorGraph, RootPlot::LP);
  setDrawOption(RootPlot::AP);

  foreach (double rigidity, rigidities) {
    TLine* line = new TLine(rigidity, 0, rigidity, 1);
    line->SetVertical();
    line->SetLineColor(kBlack);
    line->SetLineWidth(1);
    line->SetLineStyle(kDashed);
    m_lines.append(line);
  }
}

void ParameterPlot::addIntegral()
{
  TGraph* integralGraph = new TGraph(m_numberOfPoints);
  for (int point = 0; point < m_numberOfPoints; ++point) {
    double absoluteRigidity = m_min + point * m_step;
    LikelihoodPDF* pdf = m_likelihood->pdf(KineticVariable(m_particle, Enums::AbsoluteRigidity, absoluteRigidity));
    double integral = pdf->integral();
    delete pdf;
    integralGraph->SetPoint(point, absoluteRigidity, integral);
  }
  integralGraph->SetMarkerColor(kRed);
  addGraph(integralGraph, RootPlot::LP);

  qDebug() << qPrintable(m_likelihood->title() + ":");
  QString key;
  key = Enums::label(m_particle);
  key.replace("+", "%2B");
  key+= "/normalizationRigidities=";
  for (int point = 0; point < m_numberOfPoints; ++point) {
    double absoluteRigidity = m_min + point * m_step;
    if (point)
      key+= ", ";
    key+= QString::number(absoluteRigidity);
  }
  qDebug() << qPrintable(key);

  key = Enums::label(m_particle);
  key.replace("+", "%2B");
  key+= "/normalizationFactors=";
  for (int point = 0; point < m_numberOfPoints; ++point) {
    double integral = integralGraph->GetY()[point];
    if (point)
      key+= ", ";
    key+= QString::number(1./integral);
  }
  qDebug() << qPrintable(key);
  qDebug();
}

void ParameterPlot::draw(TCanvas* canvas)
{
  GraphPlot::draw(canvas);
  foreach (TLine* line, m_lines) {
    line->SetY1(yAxis()->GetXmin());
    line->SetY2(yAxis()->GetXmax());
    line->Draw();
  }
}

#include "ParameterPlot.hh"
#include "Likelihood.hh"
#include "LikelihoodPDF.hh"
#include "PDFParameters.hh"

#include <TGraph.h>

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
  int numberOfParameters = m_likelihood->numberOfParameters();

  QVector<TGraph*> extrapolatedGraphs;
  QVector<TGraph*> interpolatedGraphs;
  for (int parameter = 0; parameter < numberOfParameters; ++parameter) {
    TGraph* g = 0;
    g = new TGraph;
    g->SetName(qPrintable(QString("parameter%1 extrapolated").arg(parameter)));
    g->SetMarkerColor(kRed);
    g->SetLineColor(kRed);
    extrapolatedGraphs.append(g);
    g = new TGraph;
    g->SetName(qPrintable(QString("parameter%1 interpolated").arg(parameter)));
    g->SetMarkerColor(kBlue);
    g->SetLineColor(kBlue);
    interpolatedGraphs.append(g);
  }

  TGraph* normalizationFactorGraph = new TGraph(m_numberOfPoints);
  normalizationFactorGraph->SetLineColor(kGreen);
  normalizationFactorGraph->SetMarkerColor(kGreen);

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
  }

  foreach (TGraph* g, extrapolatedGraphs)
    if (g->GetN())
      addGraph(g, RootPlot::P);
  foreach (TGraph* g, interpolatedGraphs)
    if (g->GetN())
      addGraph(g, RootPlot::LP);
  addGraph(normalizationFactorGraph, RootPlot::LP);
  setDrawOption(RootPlot::AP);
}

ParameterPlot::~ParameterPlot()
{
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

  setAxisTitle("|R| / GV", "parameter / a.u.");

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

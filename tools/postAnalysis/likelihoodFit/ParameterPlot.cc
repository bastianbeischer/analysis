#include "ParameterPlot.hh"
#include "Likelihood.hh"
#include "LikelihoodPDF.hh"

#include <TGraph.h>

#include <QDebug>

ParameterPlot::ParameterPlot(Likelihood* likelihood, Enums::Particle particle)
  : PostAnalysisPlot()
  , GraphPlot()
{
  setTitle(QString("%1 parameters %2").arg(likelihood->title()).arg(Enums::label(particle)));
  int numberOfParameters = likelihood->numberOfParameters();

  const int numberOfPoints = 101;
  double min = 0.;
  double max = 10.;
  double step = (max - min) / (numberOfPoints - 1);

  for (int parameter = 0; parameter < numberOfParameters; ++parameter)
    addGraph(new TGraph(numberOfPoints), RootPlot::LP);

  if (numberOfParameters) {
    for (int point = 0; point < numberOfPoints; ++point) {
      double absoluteRigidity = min + point * step;
      Likelihood::ParameterVector parameters = likelihood->interpolation(Hypothesis(particle, 1. / absoluteRigidity));
      Q_ASSERT(parameters.count() == numberOfParameters);
      for (int parameter = 0; parameter < numberOfParameters; ++parameter)
        graph(parameter)->SetPoint(point, absoluteRigidity, parameters[parameter]);
    }
  }

  TGraph* integralGraph = new TGraph(numberOfPoints);
  for (int point = 0; point < numberOfPoints; ++point) {
    double absoluteRigidity = min + point * step;
    LikelihoodPDF* pdf = likelihood->pdf(KineticVariable(particle, Enums::AbsoluteRigidity, absoluteRigidity));
    double integral = pdf->integral();
    delete pdf;
    integralGraph->SetPoint(point, absoluteRigidity, integral);
  }
  integralGraph->SetMarkerColor(kRed);
  addGraph(integralGraph, RootPlot::LP);

  setAxisTitle("|R| / GV", "parameter / a.u.");

  qDebug() << qPrintable(likelihood->title() + ":");
  QString key;
  key = Enums::label(particle);
  key.replace("+", "%2B");
  key+= "/normalizationRigidities=";
  for (int point = 0; point < numberOfPoints; ++point) {
    double absoluteRigidity = min + point * step;
    if (point)
      key+= ", ";
    key+= QString::number(absoluteRigidity);
  }
  qDebug() << qPrintable(key);

  key = Enums::label(particle);
  key.replace("+", "%2B");
  key+= "/normalizationFactors=";
  for (int point = 0; point < numberOfPoints; ++point) {
    double integral = integralGraph->GetY()[point];
    if (point)
      key+= ", ";
    key+= QString::number(1./integral);
  }
  qDebug() << qPrintable(key);
  qDebug();
}

ParameterPlot::~ParameterPlot()
{
}

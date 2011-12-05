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

  const int numberOfPoints = 100;
  double min = 0.1;
  double max = 10.1;
  double step = (max - min) / (numberOfPoints - 1);

  for (int parameter = 0; parameter < numberOfParameters; ++parameter)
    addGraph(new TGraph(numberOfPoints), RootPlot::LP);

  for (int point = 0; point < numberOfPoints; ++point) {
    double absoluteRigidity = min + point * step;
    Likelihood::ParameterVector parameters = likelihood->interpolation(Hypothesis(particle, 1. / absoluteRigidity));
    Q_ASSERT(parameters.count() == numberOfParameters);
    for (int parameter = 0; parameter < numberOfParameters; ++parameter)
      graph(parameter)->SetPoint(point, absoluteRigidity, parameters[parameter]);
  }

  TGraph* integralGraph = new TGraph(numberOfPoints);
  LikelihoodPDF* pdf = likelihood->pdf(KineticVariable(particle));
  for (int point = 0; point < numberOfPoints; ++point) {
    double absoluteRigidity = min + point * step;
    pdf->setVariable(KineticVariable(particle, Enums::AbsoluteRigidity, absoluteRigidity));
    LikelihoodPDF* pdf = likelihood->pdf(KineticVariable(particle, Enums::AbsoluteRigidity, absoluteRigidity));
    double integral = pdf->integral();
    delete pdf;
    integralGraph->SetPoint(point, absoluteRigidity, integral);
  }
  integralGraph->SetMarkerColor(kRed);
  addGraph(integralGraph, RootPlot::LP);

  setAxisTitle("|R| / GV", "parameter / a.u.");
}

ParameterPlot::~ParameterPlot()
{
}

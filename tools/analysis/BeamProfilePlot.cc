#include "BeamProfilePlot.hh"

#include "Particle.hh"
#include "ParticleInformation.hh"
#include "ProjectionControlWidget.hh"
#include <QSpinBox>

#include <TH2D.h>


BeamProfilePlot::BeamProfilePlot(Type type) :
  AnalysisPlot(AnalysisPlot::Testbeam),
  H2DProjectionPlot(),
  m_type(type)
{
  controlWidget()->spinBox()->setMaximum(1024);
  int nBinsX = 0, nBinsY = 0;
  double x0 = 0., x1 = 0.;
  double y0 = 0., y1 = 0.;  
  if (m_type == Horizontal) {
    nBinsX = 100;
    x0 = -200.;
    x1 = 200.;

    nBinsY = 100;
    y0 = -0.5;
    y1 = 0.5;

    setTitle("horizontal beam profile (at z = 0)");
    setAxisTitle("y", "slope Y", "N");
  }
  else if (m_type == Vertical) {
    nBinsX = 100;
    x0 = -200.;
    x1 = 200.;

    nBinsY = 100;
    y0 = -0.05;
    y1 = 0.05;

    setTitle("vertical beam profile (at z = 0)");
    setAxisTitle("x", "slope X", "N");
  }
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsX, x0, x1, nBinsY, y0, y1);
  addHistogram(histogram);
}

BeamProfilePlot::~BeamProfilePlot()
{
}

void BeamProfilePlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  if(!track || !track->fitGood())
    return;

  // ParticleInformation::Flags flags = particle->information()->flags();
  // if (!(flags & ParticleInformation::AllTrackerLayers))
  //   return;

  if (m_type == Horizontal)
    histogram()->Fill(track->y(0), track->slopeY(0));
  else if (m_type == Vertical)
    histogram()->Fill(track->x(0), track->slopeX(0));
}

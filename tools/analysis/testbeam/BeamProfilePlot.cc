#include "BeamProfilePlot.hh"

#include "Particle.hh"
#include "ParticleInformation.hh"
#include "ProjectionControlWidget.hh"

#include <QWidget>
#include <QSpinBox>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>

#include <TH2D.h>
#include "SimpleEvent.hh"
#include "MCEventInformation.hh"
#include "MCSimpleEventParticle.hh"
#include <TVector3.h>
#include <iostream>
#include <TFile.h>

BeamProfilePlot::BeamProfilePlot(Type type) :
  H2DProjectionPlot(),
  AnalysisPlot(AnalysisPlot::Testbeam),
  m_type(type),
  m_fileNameLineEdit(new QLineEdit("beamprofile.root"))
{
  int nBinsX = 0, nBinsY = 0;
  double x0 = 0., x1 = 0.;
  double y0 = 0., y1 = 0.;  
  if (m_type == Horizontal) {
    nBinsX = 200;
    x0 = -150.;
    x1 = 150.;

    nBinsY = 200;
    y0 = -0.3;
    y1 = 0.3;

    setTitle("horizontal beam profile (at z = 0)");
    setAxisTitle("y", "slope Y", "N");
  }
  else if (m_type == Vertical) {
    nBinsX = 200;
    x0 = -150.;
    x1 = 150.;

    nBinsY = 200;
    y0 = -0.03;
    y1 = 0.03;

    setTitle("vertical beam profile (at z = 0)");
    setAxisTitle("x", "slope X", "N");
  }
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsX, x0, x1, nBinsY, y0, y1);
  addHistogram(histogram);

  ProjectionControlWidget* controlWidget = this->controlWidget();
  QPushButton* saveButton = new QPushButton("save");
  controlWidget->spinBox()->setMaximum(1024);
  controlWidget->layout()->addWidget(m_fileNameLineEdit);
  controlWidget->layout()->addWidget(saveButton);
  
  connect(saveButton, SIGNAL(clicked()), this, SLOT(saveHistograms()));
}

BeamProfilePlot::~BeamProfilePlot()
{
}

void BeamProfilePlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if(!track || !track->fitGood())
    return;

//  ParticleInformation::Flags flags = particle->information()->flags();
//  if (!(flags & ParticleInformation::AllTrackerLayers))
//    return;

  if (m_type == Horizontal)
    histogram()->Fill(track->y(0), track->slopeY(0));
  else if (m_type == Vertical)
    histogram()->Fill(track->x(0), track->slopeX(0));
}

void BeamProfilePlot::saveHistograms()
{
  QString fileName = m_fileNameLineEdit->text();
  TFile file(qPrintable(fileName),"UPDATE");
  file.cd();
  histogram()->Write(histogram()->GetName(), TObject::kOverwrite);
  std::cout << histogram()->GetName() <<"  has been saved to: "<< qPrintable(fileName)<< std::endl;
  file.Close();
}

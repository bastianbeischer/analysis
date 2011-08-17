#include "RigidityParticleSpectrum.hh"

#include <PostAnalysisCanvas.hh>

#include <TH1D.h>

#include <QString>
#include <QDebug>

RigidityParticleSpectrum::RigidityParticleSpectrum(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
{
QString name = canvas->name();
  name.replace("canvas", "original histogram");

  TH1D* histogram = new TH1D(*canvas->histograms1D().at(0));
  setTitle(name);
  addHistogram(histogram);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
}

RigidityParticleSpectrum::~RigidityParticleSpectrum()
{
}

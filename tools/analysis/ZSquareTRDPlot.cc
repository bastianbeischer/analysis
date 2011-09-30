#include "ZSquareTRDPlot.hh"
#include "BrokenLine.hh"
#include "Particle.hh"
#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Constants.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLatex.h>

#include <QDebug>

ZSquareTRDPlot::ZSquareTRDPlot()
  : AnalysisPlot(AnalysisTopic::SignalHeightTRD)
  , H2DPlot()
{
  setTitle(QString("z square TRD"));
  setAxisTitle("signal height upper TRD / ADC counts", "signal height lower TRD / ADC counts", "");
  TH2D* histogram = new TH2D(qPrintable(title()), "", 50, 0, 100, 50, 0, 100);
  addHistogram(histogram);
}

ZSquareTRDPlot::~ZSquareTRDPlot()
{
}

void ZSquareTRDPlot::processEvent(const QVector<Hit*>& clusters, const Particle* const, const SimpleEvent* const)
{
  QVector<Hit*>::const_iterator endIt = clusters.end();
  int nUpperTrd = 0;
  int nLowerTrd = 0;
  double upperSum = 0.;
  double lowerSum = 0.;
  for (QVector<Hit*>::const_iterator it = clusters.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::trd) {
      if (hit->position().z() > -411.6) {
        ++nUpperTrd;
        upperSum+= hit->signalHeight();
      } else {
        ++nLowerTrd;
        lowerSum+= hit->signalHeight();
      }
    }
  }
  histogram()->Fill(upperSum/nUpperTrd, lowerSum/nLowerTrd);
}

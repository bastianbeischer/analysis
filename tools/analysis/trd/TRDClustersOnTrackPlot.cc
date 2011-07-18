#include "TRDClustersOnTrackPlot.hh"

#include <TH1D.h>

#include "Hit.hh"
#include "Cluster.hh"
#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "TRDReconstruction.hh"
#include "RootStyle.hh"
#include "TLegend.h"

TRDClustersOnTrackPlot::TRDClustersOnTrackPlot(AnalysisPlot::Topic topic) :
  AnalysisPlot(topic),
  H1DPlot()
{
  setTitle("TRD clusters on track");
  setAxisTitle("number of trd clusters on track", "entries");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 11, -0.5, 10.5);
  histogram->SetStats(true);
  histogram->Sumw2();
  addHistogram(histogram);

  //add further histos:
  for (int i = 0; i < 7; ++i) {
    TH1D* histogram = new TH1D(qPrintable(title() + QString::number(i+1)), "", 11, -0.5, 10.5);
    histogram->SetStats(true);
    histogram->Sumw2();
    histogram->SetLineColor(2+i);
    addHistogram(histogram);
  }

  TLegend* leg = new TLegend(0.11,0.5,0.5,0.89);
  //leg->SetFillColor(kWhite);
  addLegend(leg);
}

TRDClustersOnTrackPlot::~TRDClustersOnTrackPlot()
{
}

void TRDClustersOnTrackPlot::processEvent(const QVector<Hit*>& /*hits*/,Particle* particle ,SimpleEvent*)
{
  const Track* track = particle->track();

  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  if (particle->information()->flags() & ParticleInformation::Chi2Good)
    return;

  int nTRDHits = 0;
  QVector<Hit*>::const_iterator endIt = track->hits().end();
  for (QVector<Hit*>::const_iterator it = track->hits().begin(); it != endIt; ++it) {
    if ((*it)->type() == Hit::trd)
      ++nTRDHits;
  }

  histogram(0)->Fill(nTRDHits);

  //use trdReconstruction class:
  const TRDReconstruction* trdReconstruction = particle->trdReconstruction();
  histogram(1)->Fill(trdReconstruction->getNoOfLayersWithEnergyDeposition());
  histogram(2)->Fill(trdReconstruction->getNoOfLayersWithEnergyDepositionOnTrack());
  histogram(3)->Fill(trdReconstruction->getNoOfLayersWithEnergyDepositionOnTrackAndPierced());
  histogram(4)->Fill(trdReconstruction->getNoOfLayersWithEnergyDepositionOnTrackPerLength());
  histogram(5)->Fill(trdReconstruction->getNoOfLayersWithEnergyDepositionOnTrackWithMinumLength());
  histogram(6)->Fill(trdReconstruction->getNoOfClustersOnTrack());
  histogram(7)->Fill(trdReconstruction->getNoOfHitsOnTrack());
}

void TRDClustersOnTrackPlot::update()
{
  legend()->Clear();
  legend()->AddEntry(histogram(0), qPrintable(QString("old on Track = %1").arg(QString::number(histogram(0)->GetMean(),'g',3))));
  legend()->AddEntry(histogram(1), qPrintable(QString("layers with enDep = %1").arg(QString::number(histogram(1)->GetMean(),'g',3))));
  legend()->AddEntry(histogram(2), qPrintable(QString("layers with enDep on track = %1").arg(QString::number(histogram(2)->GetMean(),'g',3))));
  legend()->AddEntry(histogram(3), qPrintable(QString("layers with enDep on track and pierced = %1").arg(QString::number(histogram(3)->GetMean(),'g',3))));
  legend()->AddEntry(histogram(4), qPrintable(QString("layers with enDep on track per length = %1").arg(QString::number(histogram(4)->GetMean(),'g',3))));
  legend()->AddEntry(histogram(5), qPrintable(QString("layers with enDep on track per min length = %1").arg(QString::number(histogram(5)->GetMean(),'g',3))));
  legend()->AddEntry(histogram(6), qPrintable(QString("clusters on track = %1").arg(QString::number(histogram(6)->GetMean(),'g',3))));
  legend()->AddEntry(histogram(7), qPrintable(QString("hits on track = %1").arg(QString::number(histogram(7)->GetMean(),'g',3))));
}

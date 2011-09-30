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

TRDClustersOnTrackPlot::TRDClustersOnTrackPlot(AnalysisTopic topic) :
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
  for (int i = 0; i < 5; ++i) {
    TH1D* histogram = new TH1D(qPrintable(title() + QString::number(i)), "", 11, -0.5, 10.5);
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

void TRDClustersOnTrackPlot::processEvent(const QVector<Hit*>& /*hits*/, const Particle* const particle, const SimpleEvent*)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  //if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
  //  return;

  histogram(0)->Fill(trdReconst->noOfClustersOnTrack());
  histogram(1)->Fill(trdReconst->noOfHitsOnTrack());
  histogram(2)->Fill(trdReconst->noOfLayersWithEnergyDeposition());
  histogram(3)->Fill(trdReconst->noOfLayersWithEnergyDepositionOnTrack());
  histogram(4)->Fill(trdReconst->noOfLayersWithEnergyDepositionOnTrackAndPierced());
}

void TRDClustersOnTrackPlot::update()
{
  legend()->Clear();
  legend()->AddEntry(histogram(0), qPrintable(QString("clusters on track = %1").arg(QString::number(histogram(0)->GetMean(),'g',3))));
  legend()->AddEntry(histogram(1), qPrintable(QString("hits on track = %1").arg(QString::number(histogram(1)->GetMean(),'g',3))));
  legend()->AddEntry(histogram(2), qPrintable(QString("layers with enDep = %1").arg(QString::number(histogram(2)->GetMean(),'g',3))));
  legend()->AddEntry(histogram(3), qPrintable(QString("layers with enDep on track = %1").arg(QString::number(histogram(3)->GetMean(),'g',3))));
  legend()->AddEntry(histogram(4), qPrintable(QString("layers with enDep on track and pierced = %1").arg(QString::number(histogram(4)->GetMean(),'g',3))));
}

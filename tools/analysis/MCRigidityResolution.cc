#include "MCRigidityResolution.hh"

#include <TH1D.h>
#include <TF1.h>
#include <TLatex.h>

#include <CLHEP/Units/SystemOfUnits.h>

#include "SimpleEvent.hh"
#include "Track.hh"
#include "TrackInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"

MCRigidityResolution::MCRigidityResolution()
  : AnalysisPlot(AnalysisPlot::MonteCarloTracker)
  , H1DPlot()
{
  setTitle("momentum resolution");

  TH1D* hist = new TH1D(qPrintable(title()), qPrintable(title()+";1/R;entries"), 100, -2, 2);
  addHistogram(hist);

  addFunction(new TF1("rigiditygausfit", "gaus"));

  addLatex(RootPlot::newLatex(.55, .85));
  addLatex(RootPlot::newLatex(.55, .82));
}


void MCRigidityResolution::processEvent(const QVector<Hit*>& /*hits*/, Track* track, SimpleEvent* event)
{
  //only accept mc events:
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;


  //check if everything worked and a track has been fit
  if (!track || !track->fitGood())
    return;

  //check if track fits chi2 is ok
  if (!(track->information()->flags() & TrackInformation::Chi2Good))
    return;

  //check if track was inside of magnet
  if (!(track->information()->flags() & TrackInformation::InsideMagnet))
    return;

  //
  if (event->MCInformation()->primary()->pdgID != 2212)
    return;


  //get mc rigidity
  double mcMom = event->MCInformation()->primary()->initialMomentum.Mag();

  double mcRigidity = mcMom;

  if(mcRigidity < 0.9*CLHEP::GeV || mcRigidity > 1.1*CLHEP::GeV)
    return;

  //get the reconstructed momentum
  double rigidity = track->rigidity() *CLHEP::GeV; //GeV

  double innverseDifference = 1.0 /(rigidity/CLHEP::GeV) - 1.0 /(mcRigidity/CLHEP::GeV);

  histogram()->Fill(innverseDifference);
}

void MCRigidityResolution::update()
{
 histogram()->Fit(function()) ;
 latex(0)->SetTitle(qPrintable(QString("RMS = %1+-%2").arg(histogram()->GetRMS()).arg(histogram()->GetRMSError())));
 latex(1)->SetTitle(qPrintable(QString("#sigma = %1+-%2").arg(function()->GetParameter(2)).arg(function()->GetParError(2))));
}

void MCRigidityResolution::finalize()
{
  update();
}

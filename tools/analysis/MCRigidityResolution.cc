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

MCRigidityResolution::MCRigidityResolution(int pdgID)
  : AnalysisPlot(AnalysisPlot::MonteCarloTracker)
  , H1DPlot()
  , m_pdgID(pdgID)
  , m_rigidityRangeLower(0)
  , m_rigidityRangeUppper(12)
  , m_numberOfBins(50)
{
  setTitle("rigidity resolution for " + QString::number(m_pdgID));

  TH1D* hist = new TH1D(qPrintable(title()), qPrintable(title())
                        , m_numberOfBins
                        , m_rigidityRangeLower
                        , m_rigidityRangeUppper);
  setAxisTitle("R / GeV","resolution");
  addHistogram(hist);

 for (int i = 1; i <= hist->GetNbinsX(); ++i)
  {
   QString histTitle = QString("resolutionhist_%1_%2").arg(m_pdgID).arg(i);
   double inverseRigRange = 1. / (3. * 3.);
   int resolutionBins = 100;
   m_resolutionHistos.insert(i, new TH1D(qPrintable(histTitle)
                                         ,qPrintable(histTitle)
                                         , resolutionBins, -inverseRigRange, inverseRigRange));

  }
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

  //only use selected pdgID
  if (event->MCInformation()->primary()->pdgID != m_pdgID)
    return;


  //get mc rigidity
  double mcMom = event->MCInformation()->primary()->initialMomentum.Mag();

  double mcRigidity = mcMom;

  //get the reconstructed momentum
  double rigidity = track->rigidity() *CLHEP::GeV; //GeV

  double inverseDifference = 1.0 /(rigidity/CLHEP::GeV) - 1.0 /(mcRigidity/CLHEP::GeV); // 1 /GV

  //get bin
  int bin = histogram()->FindBin(mcRigidity / CLHEP::GeV);


  if (m_resolutionHistos.contains(bin))
  {
    TH1D* hist = m_resolutionHistos.value(bin);
    hist->Fill(inverseDifference);
  }
}

void MCRigidityResolution::update()
{
  histogram()->Reset();

  QMap<int, TH1D*>::const_iterator i;
  for (i = m_resolutionHistos.constBegin(); i != m_resolutionHistos.constEnd(); ++i)
  {
    TH1D* hist = i.value();
    double mcRigidity = histogram()->GetBinCenter(i.key());
    double inverseRMS = hist->GetRMS();
    double inverseRMSError = hist->GetRMSError();
    double rigidityRes = inverseRMS / (1. / mcRigidity) ;
    double rigidityResErr = inverseRMSError / (1. / mcRigidity) ;

    histogram()->SetBinContent(i.key(), rigidityRes);
    histogram()->SetBinError(i.key(), rigidityResErr);
  }


}

void MCRigidityResolution::finalize()
{
  update();
}

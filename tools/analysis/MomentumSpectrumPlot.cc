#include "MomentumSpectrumPlot.hh"

#include <TH1D.h>
#include <TLatex.h>

#include "Hit.hh"
#include "ParticleInformation.hh"
#include "Particle.hh"
#include "Track.hh"
#include "SimpleEvent.hh"
#include "Helpers.hh"

#include <iostream>
#include <cmath>

MomentumSpectrumPlot::MomentumSpectrumPlot(Enums::ChargeSigns type, bool inverted)
  : AnalysisPlot(AnalysisTopic::MomentumReconstruction)
  , H1DPlot()
  , m_type(type)
  , m_inverted(inverted)
{
  QString title = "momentum spectrum";
  
  if (inverted) {
    title+= " - Inverted";
    setAxisTitle("1/R / 1/GV", "particles / bin");
  } else {
    setAxisTitle("R / GV", "particles / GV");
  }
  setTitle(title);

  int nBins = inverted ? 1200 : 100;
  TH1D* histogram = 0;
  if (m_type == (Enums::Positive | Enums::Negative)) {
    title+= " - All";
    histogram = new TH1D(qPrintable(title), "", nBins, -20., 20.);
  } else {
    if (m_type == Enums::Negative)
      title+= " - Negative";
    if (m_type == Enums::Positive)
      title+= " - Positive";
    const QVector<double>& axis = Helpers::logBinning(nBins, 0.1, 20.);
    histogram = new TH1D(qPrintable(title), "", nBins, axis.constData());
  }
  
  histogram->SetLineColor(kBlack);
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(.15, .85));
  if (inverted) {
    addLatex(RootPlot::newLatex(.15, .75));
    addLatex(RootPlot::newLatex(.15, .65));
  }
}

MomentumSpectrumPlot::~MomentumSpectrumPlot()
{
}

void MomentumSpectrumPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::AllTrackerLayers) || !(flags & ParticleInformation::InsideMagnet) || (flags & ParticleInformation::Albedo) )
    return;

  double rigidity = track->rigidity();
  if (m_type == Enums::Negative && rigidity >= 0)
    return;
  if (m_type == Enums::Positive && rigidity <= 0)
    return;

  double value = rigidity;
  if (m_type == Enums::Negative) {
    value = -rigidity;
  }
  else if (m_inverted) {
    value = 1./rigidity;
  }

  int iBin = histogram()->FindBin(value);
  double width = histogram()->GetBinWidth(iBin);
  double weight = 1./width;
  if (m_inverted) {
    weight = 1.;
  }

  histogram()->Fill(value, weight);
}

void MomentumSpectrumPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("entries = %1").arg(histogram()->GetEntries())));
  if (m_inverted) {
    double mean = histogram()->GetMean();
    double rms = histogram()->GetRMS();
    latex(1)->SetTitle(qPrintable(QString("#mu_R = %1 GV").arg(1./mean, 0, 'f', 2)));
    latex(2)->SetTitle(qPrintable(QString("#sigma_{R}/R = %1").arg(rms/fabs(mean), 0, 'f', 2)));
  }
}

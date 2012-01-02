#include "LikelihoodPdfFitPlot.hh"
#include "Likelihood.hh"
#include "LikelihoodPDF.hh"

#include <TH2.h>
#include <TAxis.h>

#include <QDebug>

LikelihoodPdfFitPlot::LikelihoodPdfFitPlot(const Likelihood* lh, const TH2D* h, int bin)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_currentFunction(0)
  , m_previewFunction(0)
{
  double rangeMin = h->GetXaxis()->GetBinLowEdge(bin);
  double rangeMax = h->GetXaxis()->GetBinUpEdge(bin);
  double center = .5 * (rangeMin + rangeMax);

  Enums::Particles particles = lh->particles();

  QString title = QString("%1 PDF %2..%3 GeV %4").arg(lh->title()).arg(rangeMin, 3, 'f', 1, '0')
    .arg(rangeMax, 3, 'f', 1, '0').arg(Enums::label(particles));
  setTitle(title);

  for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it) {
    if (!(it.key() & particles))
      continue;
    addHistogram(h->ProjectionY(qPrintable(title), bin, bin));
    m_currentFunction = lh->pdf(KineticVariable(it.key(), Enums::AbsoluteRigidity, center));
    addFunction(m_currentFunction);
    m_previewFunction = lh->pdf(KineticVariable(it.key(), Enums::AbsoluteRigidity, center));
    addFunction(m_previewFunction);
  }
}

LikelihoodPdfFitPlot::~LikelihoodPdfFitPlot()
{
}

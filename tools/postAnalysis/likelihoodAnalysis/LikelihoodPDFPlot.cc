#include "LikelihoodPDFPlot.hh"
#include "Likelihood.hh"
#include "LikelihoodPDF.hh"
#include "SignalHeightTrdLikelihood.hh"
#include "ParameterWidget.hh"

#include <TPad.h>
#include <TLatex.h>

#include <QDebug>
#include <QLayout>

LikelihoodPDFPlot::LikelihoodPDFPlot(Enums::LikelihoodVariable type, Enums::Particles particles, int layer)
  : QObject()
  , PostAnalysisPlot()
  , H1DPlot()
  , m_likelihood(Likelihood::newLikelihood(type, particles))
  , m_pdfs()
{
  if (type == Enums::SignalHeightTrdLikelihood)
    static_cast<SignalHeightTrdLikelihood*>(m_likelihood)->setLayer(layer);
  setTitle(m_likelihood->title() + " PDF");
  QVector<Enums::Particle> particleVector = Enums::particleVector(particles);
  foreach (Enums::Particle particle, particleVector) {
    LikelihoodPDF* pdf = m_likelihood->pdf(KineticVariable(particle, Enums::AbsoluteRigidity, 1.));
    Q_ASSERT(pdf);
    addFunction(pdf);
    m_pdfs.insert(particle, pdf);
  }
  addLatex(RootPlot::newLatex(0.12, 0.90));
  ParameterWidget* widget = new ParameterWidget;
  widget->setRange(0, 10.);
  setSecondaryWidget(widget);
  connect(widget, SIGNAL(valueChanged(double)), this, SLOT(rigidityChanged(double)));
  widget->setValue(3.0);
}

LikelihoodPDFPlot::~LikelihoodPDFPlot()
{
  delete m_likelihood;
}

void LikelihoodPDFPlot::rigidityChanged(double rigidity)
{
  QMapIterator<Enums::Particle, LikelihoodPDF*> it(m_pdfs);
  while (it.hasNext()) {
    it.next();
    Hypothesis(it.key(), 1./rigidity);
    const PDFParameters& parameters = m_likelihood->interpolation(Hypothesis(it.key(), 1./rigidity));
    it.value()->setParameters(parameters);
  }
  latex()->SetTitle(qPrintable(QString("%1 %2 GV").arg(m_likelihood->title()).arg(rigidity, 4, 'f', 2, '0')));
  gPad->Modified();
  gPad->Update();
}

#include "LikelihoodFitWindow.hh"
#include "LikelihoodPDFFitPlot.hh"
#include "Setup.hh"
#include "NormalizationThread.hh"
#include "Likelihood.hh"
#include "KineticVariable.hh"
#include "LikelihoodPDF.hh"

#include <QDebug>
#include <QPushButton>
#include <QWidget>
#include <QComboBox>

LikelihoodFitWindow::LikelihoodFitWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
  , m_particles(Setup::instance()->proposedParticles())
  , m_fitPlots()
  , m_comboBox(new QComboBox)
  , m_likelihoods()
  , m_results()
  , m_runningThreadsCounter(0)
{
  m_comboBox->addItem("all (bad + good + low statistics)");
  m_comboBox->addItem("bad");
  m_comboBox->addItem("good");
  m_comboBox->addItem("low statistics");
  m_comboBox->addItem("bad + good");
  m_comboBox->addItem("bad + low statistics");
  m_comboBox->addItem("good + low statistics");
  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(plotSelectionChanged()));
  addWidget(m_comboBox);

  QPushButton* button = 0;
  button = new QPushButton("save");
  connect(button, SIGNAL(clicked()), this, SLOT(save()));
  addWidget(button);

  button = new QPushButton("normalize all");
  connect(button, SIGNAL(clicked()), this, SLOT(normalizeAll()));
  addWidget(button);

  button = new QPushButton("fit all");
  connect(button, SIGNAL(clicked()), this, SLOT(fitAll()));
  addWidget(button);
}

LikelihoodFitWindow::~LikelihoodFitWindow()
{
  qDeleteAll(m_likelihoods);
}

void LikelihoodFitWindow::configFileChanged()
{
  foreach (LikelihoodPDFFitPlot* plot, m_fitPlots)
    plot->update();
}

void LikelihoodFitWindow::fitAll()
{
  foreach (LikelihoodPDFFitPlot* plot, m_fitPlots)
    plot->fit();
}

void LikelihoodFitWindow::save()
{
  foreach (LikelihoodPDFFitPlot* plot, m_fitPlots)
    if (passes(plot))
      plot->save();
}

bool LikelihoodFitWindow::passes(const LikelihoodPDFFitPlot* plot)
{
  const QString& text = m_comboBox->currentText();
  return (plot->bad() && text.contains("bad")) || (plot->good() && text.contains("good"))
    || (plot->lowStatistics() && text.contains("low statistics"));
}

void LikelihoodFitWindow::plotSelectionChanged()
{
  clearPlots();
  foreach (PostAnalysisPlot* plot, m_otherPlots)
    addPlot(plot);
  foreach (LikelihoodPDFFitPlot* plot, m_fitPlots)
    if (passes(plot))
      addPlot(plot);
}

void LikelihoodFitWindow::normalizeAll()
{
  Q_ASSERT(m_likelihoods.count());
  QVector<LikelihoodPDF*> pdfs;
  m_results.clear();
  foreach (Likelihood* lh, m_likelihoods) {
    QVector<Enums::Particle> particles;
    for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it)
      if (it.key() & lh->particles())
        particles.append(it.key());
    Q_ASSERT(particles.count() == 1);
    Enums::Particle particle = particles.first();
    if (lh->parametrizationAvailable()) {
      double min = lh->parametrizationMin();
      double max = lh->parametrizationMax();
      double step = lh->parametrizationStep();
      int numberOfElements = (max - min) / step + 1;
      for (int i = 0; i < numberOfElements; ++i) {
        double rigidity = min + i * step;
        if (i == numberOfElements - 1) // necessary to prevent rounding error
          rigidity = max;
        pdfs.append(lh->pdf(KineticVariable(particle, Enums::AbsoluteRigidity, rigidity)));
      }
    }
    m_results.insert(particle, ResultVector());
  }
  Q_ASSERT(m_runningThreadsCounter == 0);
  int nThreads = QThread::idealThreadCount();
  Q_ASSERT(nThreads > 0);
  int fitsPerThread = pdfs.count() / nThreads;
  int remaining = pdfs.size() - nThreads * fitsPerThread;
  int from = 0;
  setEnabled(false);
  for (int i = 0; i < nThreads; ++i) {
    int fitsForThisThread = (remaining <= i) ? fitsPerThread : fitsPerThread + 1;
    QVector<LikelihoodPDF*> subPdfs = pdfs.mid(from, fitsForThisThread);
    from+= fitsForThisThread;
    NormalizationThread* thread = new NormalizationThread(subPdfs);
    connect(thread, SIGNAL(finished()), this, SLOT(normalizationFinished()));
    ++m_runningThreadsCounter;
    thread->start();
  }
}

bool LikelihoodFitWindow::lessThan(const QPointF& first, const QPointF& second)
{
  Q_ASSERT(!qFuzzyCompare(first.x(), second.x()));
  if (first.x() < second.x())
    return true;
  return false;
}

void LikelihoodFitWindow::normalizationFinished()
{
  --m_runningThreadsCounter;
  NormalizationThread* thread = static_cast<NormalizationThread*>(sender());
  const QVector<LikelihoodPDF*>& pdfs = thread->pdfs();
  const QVector<double>& results = thread->results();
  int n = pdfs.size();
  for (int i = 0; i < n; ++i)
    m_results[pdfs[i]->variable().particle()].append(QPointF(pdfs[i]->variable().absoluteRigidity(), results[i]));
  delete thread;
  if (m_runningThreadsCounter == 0) {
    setEnabled(true);
    QMutableMapIterator<Enums::Particle, ResultVector> iterator(m_results);
    while (iterator.hasNext()) {
      iterator.next();
      ResultVector& vector = iterator.value();
      qSort(vector.begin(), vector.end(), lessThan);
      QVector<double> rigidities;
      QVector<double> normalizationFactors;
      foreach (const QPointF& point, vector) {
        rigidities.append(point.x());
        normalizationFactors.append(point.y());
      }
      Likelihood* lh = 0;
      foreach (lh, m_likelihoods)
        if (lh->particles() == iterator.key())
          break;
      Q_ASSERT(lh);
      if (rigidities.count())
        lh->saveNormalization(iterator.key(), rigidities, normalizationFactors);
    }
  }
}

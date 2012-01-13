#include "NormalizationThread.hh"
#include "LikelihoodPDF.hh"

#include <QDebug>

NormalizationThread::NormalizationThread(const QVector<LikelihoodPDF*>& pdfs, QObject* parent)
  : QThread(parent)
  , m_pdfs(pdfs)
  , m_results()
{
}

NormalizationThread::~NormalizationThread()
{
  qDeleteAll(m_pdfs);
}

const QVector<LikelihoodPDF*>& NormalizationThread::pdfs() const
{
  return m_pdfs;
}

const QVector<double>& NormalizationThread::results() const
{
  return m_results;
}

void NormalizationThread::run()
{
  foreach (LikelihoodPDF* pdf, m_pdfs) {
    pdf->setNormalizationFactor(1.);
    m_results.append(1./pdf->integral());
  }
}

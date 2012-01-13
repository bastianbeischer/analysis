#ifndef NormalizationThread_hh
#define NormalizationThread_hh

#include "Enums.hh"

#include <QThread>
#include <QVector>

class LikelihoodPDF;

class NormalizationThread : public QThread
{
Q_OBJECT
public:
  NormalizationThread(const QVector<LikelihoodPDF*>&, QObject* = 0);
  virtual ~NormalizationThread();
  const QVector<LikelihoodPDF*>& pdfs() const;
  const QVector<double>& results() const;
protected:
  virtual void run();
protected:
  QVector<LikelihoodPDF*> m_pdfs;
  QVector<double> m_results;
};

#endif /* NormalizationThread_hh */

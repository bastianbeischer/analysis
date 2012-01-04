#ifndef PreAnalysis_hh
#define PreAnalysis_hh

#include <QMap>
#include <QVector>

class DetectorElement;
class Hit;
class SimpleEvent;

typedef QMap< DetectorElement*, QVector<Hit*> > HitStorage;
typedef QMap< DetectorElement*, QVector<Hit*> >::iterator HitStorageIterator;

class PreAnalysis
{

public:
  PreAnalysis();
  ~PreAnalysis();

  SimpleEvent* generateCompressedEvent(SimpleEvent* event);

private:
  HitStorage m_hitStorage;

};

#endif /* PreAnalysis_hh */

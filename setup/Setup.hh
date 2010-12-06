#ifndef Setup_hh
#define Setup_hh

#include <QMap>
#include <QVector>
#include <QFlags>

#include <TVector3.h>

class Cluster;
class Hit;
class Layer;
class DetectorElement;
class QSettings;

typedef QMap<double,Layer*>::iterator LayerIterator;
typedef QMap<unsigned short,DetectorElement*>::iterator ElementIterator;

class Setup
{
  
public:
  enum CorrectionFlag {None = 0x0, Alignment = 0x1, TimeShifts = 0x2};
  Q_DECLARE_FLAGS(CorrectionFlags, CorrectionFlag);

public:
  ~Setup();

  static Setup*     instance();
  
public:
  Layer*            layer(double z);
  DetectorElement*  element(unsigned short id);

public:
  Layer*            firstLayer();
  DetectorElement*  firstElement();
  Layer*            nextLayer();
  DetectorElement*  nextElement();

public:
  QVector<Hit*>     generateClusters(const QVector<Hit*>& hits);
  void              addHitsToLayers(const QVector<Hit*>& hits);
  void              clearHitsFromLayers();
  void              applyCorrections(QVector<Hit*>& hits, CorrectionFlags flags = ~CorrectionFlags(0));

public:
  TVector3          configFilePosition(QString group, unsigned short detId) const;
  double            configFileAlignmentShift(QString group, unsigned short detId) const;
  double            configFileTimeShift(unsigned short detId) const;

public:
  void              writeSettings();

private:
  Setup();
  
private:
  void              construct();
  TVector3          positionForHit(const Hit* hit);
  double            timeShiftForHit(const Hit* hit);

private:
  static Setup*                          m_instance;

  QSettings*                             m_coordinates;
  QSettings*                             m_settings;

  LayerIterator                          m_layerIt;
  ElementIterator                        m_elementIt;

  QMap<double, Layer*>                   m_layers;
  QMap<unsigned short, DetectorElement*> m_elements;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Setup::CorrectionFlags);

#endif /* Setup_hh */

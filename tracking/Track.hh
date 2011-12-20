#ifndef Track_hh
#define Track_hh

#include "Matrix.hh"
#include "Enums.hh"

#include <TVector3.h>

#include <QVector>
#include <QMap>
#include <QDebug>

class Hit;

class Track {
public:
  Track();
  virtual ~Track();

public:
  void setVerbose(unsigned short verb) {m_verbose = verb;}

public:
  Enums::TrackType type() const {return m_type;}
  unsigned short verbose() const {return m_verbose;}
  int fitGood() const {return m_fitGood;}
  unsigned short nParameters() const {return m_matrix->nCol();}
  double chi2() const {return m_chi2;}
  unsigned int ndf() const {return m_ndf;}
  double transverseRigidity() const {return m_transverseRigidity;}
  double trackLength() const {return m_trackLength;}
  double signalHeight() const {return m_signalHeight;}
  const QVector<Hit*>& hits() const {return m_hits;}

  virtual double x(double z) const = 0;
  virtual double y(double z) const = 0;
  virtual double slopeX(double z) const = 0;
  virtual double slopeY(double z) const = 0;
  virtual double bendingAngle() const = 0;
  
  double azimuthAngle() const;
  double zenithAngle() const;
  
  TVector3 position(double z) const {return TVector3(x(z), y(z), z);}

  TVector3 meanFieldAlongTrack();
  double rigidity() const;

public:
  void reset();
  int fit(const QVector<Hit*>& hits);

private:
  virtual void retrieveFitResults() = 0;
  virtual void calulateTrackLength() = 0;
  void calculateTransverseRigidity();

protected:
  Enums::TrackType m_type;

  Matrix* m_matrix;

  unsigned short m_verbose;

  int m_fitGood;
  double m_chi2;
  unsigned int m_ndf;

  double m_transverseRigidity;
  double m_trackLength;
  double m_signalHeight;
  QVector<Hit*> m_hits;
};

#endif /* Track_hh */

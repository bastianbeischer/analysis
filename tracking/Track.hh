#ifndef Track_hh
#define Track_hh

#include "Matrix.hh"

#include <TVector3.h>

#include <QVector>
#include <QMap>

class Hit;

class Track {
public:
  Track();
  virtual ~Track();

public:
  enum Type {None=-1, StraightLine, BrokenLine, CenteredBrokenLine, CenteredBrokenLine2D};

  void setVerbose(unsigned short verb) {m_verbose = verb;}

public:
  Type type() const {return m_type;}
  unsigned short verbose() const {return m_verbose;}
  int fitGood() const {return m_fitGood;}
  unsigned short nParameters() const {return m_matrix->nCol();}
  double chi2() const {return m_chi2;}
  unsigned int ndf() const {return m_ndf;}
  double transverseRigidity() const {return m_transverseRigidity;}
  const QVector<Hit*>& hits() const {return m_hits;}

  virtual double x(double z) const = 0;
  virtual double y(double z) const = 0;
  virtual double slopeX(double z) const = 0;
  virtual double slopeY(double z) const = 0;
  virtual double bendingAngle() const = 0;
  virtual double trackLength() const = 0;
  
  virtual double upperAzimuth() const = 0;
  virtual double upperZenith() const = 0;
  
  TVector3 position(double z) const {return TVector3(x(z), y(z), z);}

  TVector3 meanFieldAlongTrack();
  double rigidity() const;

public:
  void reset();
  int fit(const QVector<Hit*>& hits);

private:
  virtual void retrieveFitResults() = 0;
  void calculateTransverseRigidity();

protected:
  Type m_type;

  Matrix* m_matrix;

  unsigned short m_verbose;

  int m_fitGood;
  double m_chi2;
  unsigned int m_ndf;

  double m_transverseRigidity;

  QVector<Hit*> m_hits;
};

#endif /* Track_hh */

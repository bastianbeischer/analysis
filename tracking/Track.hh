#ifndef Track_hh
#define Track_hh

#include <QVector>
#include "Matrix.hh"

class Hit;
class TVector3;

class Track
{
  
public:
  Track();
  virtual ~Track();

public:
  enum Type {None, StraightLine, BrokenLine, CenteredBrokenLine};

  void             setVerbose(unsigned short verb) {m_verbose = verb;}

public:
  Type             type()             const {return m_type;}
  unsigned short   verbose()          const {return m_verbose;}
  int              fitGood()          const {return m_fitGood;}
  unsigned short   nParameters()      const {return m_matrix->nCol();}
  double           chi2()             const {return m_chi2;}
  unsigned int     ndf()              const {return m_ndf;}
  const QVector<Hit*>& hits()         const {return m_hits;}
  
  virtual double   x(double z)        const = 0;
  virtual double   y(double z)        const = 0;
  virtual TVector3 position(double z) const = 0;
  virtual double   bendingAngle()     const = 0;
  double           pt()               const;

public:
  virtual int      fit   (const QVector<Hit*>& hits) = 0;

protected:
  Type              m_type;

  Matrix*           m_matrix;

  unsigned short    m_verbose;

  int               m_fitGood;
  double            m_chi2;
  unsigned int      m_ndf;

  QVector<Hit*>     m_hits;

};

#endif /* Track_hh */

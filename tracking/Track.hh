#ifndef Track_hh
#define Track_hh

#include <QVector>

#include <TVector3.h>

class Hit;

class Track
{
  
public:
  Track();
  ~Track();

public:
  void             setVerbose(unsigned short verb) {m_verbose = verb;}

public:
  unsigned short   verbose()          const {return m_verbose;}
  double           chi2()             const {return m_chi2;}
  unsigned int     ndf()              const {return m_ndf;}
  QVector<Hit*>    hits()             const {return m_hits;}

  virtual double   x(double z)        const = 0;
  virtual double   y(double z)        const = 0;
  virtual TVector3 position(double z) const = 0;

public:
  virtual int      fit   (QVector<Hit*> hits) = 0;

protected:
  unsigned short    m_verbose;

  double            m_chi2;
  unsigned int      m_ndf;

  QVector<Hit*>     m_hits;

};

#endif /* Track_hh */

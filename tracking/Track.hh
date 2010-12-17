#ifndef Track_hh
#define Track_hh

#include <QVector>
#include "Matrix.hh"
#include <TVector3.h>

class TrackInformation;
class Hit;

class Track
{
  
public:
  Track();
  virtual ~Track();

public:
  enum Type {None=-1, StraightLine, BrokenLine, CenteredBrokenLine};

  void             setVerbose(unsigned short verb) {m_verbose = verb;}

public:
  Type                    type()         const {return m_type;}
  const TrackInformation* information()  const {return m_information;}
  unsigned short          verbose()      const {return m_verbose;}
  int                     fitGood()      const {return m_fitGood;}
  unsigned short          nParameters()  const {return m_matrix->nCol();}
  double                  chi2()         const {return m_chi2;}
  unsigned int            ndf()          const {return m_ndf;}
  double                  pt()           const {return m_pt;}
  double                  timeOfFlight() const {return m_timeOfFlight;}
  const QVector<Hit*>&    hits()         const {return m_hits;}
  
  virtual double   x(double z)        const = 0;
  virtual double   y(double z)        const = 0;
  virtual double   slopeX(double z)   const = 0;
  virtual double   slopeY(double z)   const = 0;
  virtual double   bendingAngle()     const = 0;
  TVector3         position(double z) const {return TVector3(x(z), y(z), z);}
  double           p()                const;

public:
  int process(const QVector<Hit*>& hit);
  
private:
  void             calculatePt();
  void             calculateTimeOfFlight();
  virtual int      fit (const QVector<Hit*>& hits) = 0;

protected:
  Type              m_type;

  TrackInformation* m_information;
  Matrix*           m_matrix;

  unsigned short    m_verbose;

  int               m_fitGood;
  double            m_chi2;
  unsigned int      m_ndf;

  double            m_pt;
  double            m_timeOfFlight;

  QVector<Hit*>     m_hits;

};

#endif /* Track_hh */

#ifndef Likelihood_hh
#define Likelihood_hh

#include "Enums.hh"

#include <QMap>
#include <QVector>
#include <QDebug>

class LikelihoodPDF;
class LikelihoodRatio;

class Likelihood {
public:
  typedef QVector<double> ParameterVector;
  typedef QMap<double, ParameterVector> MomentumMap;
  typedef QMap<Enums::Particle, MomentumMap> ParticleMap;

  Likelihood();
  ~Likelihood();
  void setType(Enums::LikelihoodVariable);
  Enums::LikelihoodVariable type() const;

  virtual double min() const = 0;
  virtual double max() const = 0;
  virtual int numberOfParameters() const = 0;
  virtual double eval(double p, Enums::Particle, double realMomentum) const = 0;
  
  virtual LikelihoodPDF* pdf(Enums::Particle, double realMomentum) const;
  virtual LikelihoodRatio* ratio(Enums::Particle, double realMomentum) const;
  virtual double ratio(double p, Enums::Particle, double realMomentum) const;

  Enums::Particles particles() const;
  
  MomentumMap::ConstIterator end(Enums::Particle) const;
  MomentumMap::ConstIterator lowerNode(Enums::Particle, double realMomentum) const;
  MomentumMap::ConstIterator upperNode(Enums::Particle, double realMomentum) const;

  QVector<double> linearInterpolation(Enums::Particle, double realMomentum) const;
private:
  virtual void loadNodes();

  Enums::LikelihoodVariable m_type;
  Enums::Particles m_particles;
  ParticleMap m_nodes;
};

#endif

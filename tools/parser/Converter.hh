#ifndef Converter_hh
#define Converter_hh

class SimpleEvent;
class SingleFile;

class MCParticle;
class MCEvent;

class MCSimpleEvent;
class MCSingleFile;
class MCEventInformation;
class MCSimpleEventParticle;
class MCSimpleEventDigi;

#include <QMap>
#include <TVector3.h>

class Converter
{
  
public:
  Converter();
  ~Converter();
  
public:
  SimpleEvent* generateNextSimpleEvent(const SingleFile* file, const MCSingleFile* mcFile);

private:
  const MCEventInformation* generateNextMCEventInformation(const MCSingleFile* mcFile);
  const MCSimpleEventParticle* constructMCSimpleEventParticle(MCParticle* mcParticle);
  const QVector<const MCSimpleEventDigi*> getAllMCDigis(const MCEvent*);
  
private:
  QMap<unsigned short, TVector3> m_positions;
  QMap<unsigned short, TVector3> m_counterPositions;

};

#endif /* Converter_hh */

#ifndef TOFSensorTypeAssignment_hh
#define TOFSensorTypeAssignment_hh

#include "SensorTypes.hh"

#include <QMap>
#include <QList>

class TOFSensorTypeAssignment {
public:
	TOFSensorTypeAssignment();
	~TOFSensorTypeAssignment();
	
	SensorTypes::Type tofSensorType(unsigned int tofChannel);
	
private:
	QMap<SensorTypes::Type, QList<unsigned int> > m_sensorTypeTofChannelAssignment;
	
};

#endif /* TOFSensorTypeAssignment_hh */

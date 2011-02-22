#include "TOFSensorTypeAssignment.hh"

#include <qdebug.h>

TOFSensorTypeAssignment::TOFSensorTypeAssignment() {
	QList<unsigned int> tof1;
	tof1.push_back(0x8000|8); 
	tof1.push_back(0x8000|9); 
	tof1.push_back(0x8000|12); 
	tof1.push_back(0x8000|13); 
	tof1.push_back(0x8000|24); 
	tof1.push_back(0x8000|25); 
	tof1.push_back(0x8000|28); 
	tof1.push_back(0x8000|29);
	
	QList<unsigned int> tof2;
	tof2.push_back( 0x8000|0 ); 
	tof2.push_back( 0x8000|1 ); 
	tof2.push_back( 0x8000|4 ); 
	tof2.push_back( 0x8000|5 ); 
	tof2.push_back( 0x8000|16); 
	tof2.push_back( 0x8000|17); 
	tof2.push_back( 0x8000|20); 
	tof2.push_back( 0x8000|21);
	
	QList<unsigned int> tof3;
	tof3.push_back( 0x8000|11); 
	tof3.push_back( 0x8000|10); 
	tof3.push_back( 0x8000|15); 
	tof3.push_back( 0x8000|14); 
	tof3.push_back( 0x8000|27); 
	tof3.push_back( 0x8000|26); 
	tof3.push_back( 0x8000|30); 
	tof3.push_back( 0x8000|31);
	
	QList<unsigned int> tof4;
	tof4.push_back( 0x8000|3 ); 
	tof4.push_back( 0x8000|2 ); 
	tof4.push_back( 0x8000|7 ); 
	tof4.push_back( 0x8000|6 ); 
	tof4.push_back( 0x8000|19); 
	tof4.push_back( 0x8000|18); 
	tof4.push_back( 0x8000|23); 
	tof4.push_back( 0x8000|22);
	
	QList<unsigned int> tof5;
	tof5.push_back( 0x8000|(8+32) ); 
	tof5.push_back( 0x8000|(9+32) ); 
	tof5.push_back( 0x8000|(12+32)); 
	tof5.push_back( 0x8000|(13+32)); 
	tof5.push_back( 0x8000|(24+32)); 
	tof5.push_back( 0x8000|(25+32)); 
	tof5.push_back( 0x8000|(28+32)); 
	tof5.push_back( 0x8000|(29+32));
	
	QList<unsigned int> tof6;
	tof6.push_back( 0x8000|(0+32) ); 
	tof6.push_back( 0x8000|(1+32) ); 
	tof6.push_back( 0x8000|(4+32) ); 
	tof6.push_back( 0x8000|(5+32) ); 
	tof6.push_back( 0x8000|(16+32)); 
	tof6.push_back( 0x8000|(17+32)); 
	tof6.push_back( 0x8000|(20+32)); 
	tof6.push_back( 0x8000|(21+32));
	
	QList<unsigned int> tof7;
	tof7.push_back( 0x8000|(11+32)); 
	tof7.push_back( 0x8000|(10+32)); 
	tof7.push_back( 0x8000|(15+32)); 
	tof7.push_back( 0x8000|(14+32)); 
	tof7.push_back( 0x8000|(27+32)); 
	tof7.push_back( 0x8000|(26+32)); 
	tof7.push_back( 0x8000|(30+32)); 
	tof7.push_back( 0x8000|(31+32));
	
	QList<unsigned int> tof8;
	tof8.push_back( 0x8000|(3+32) ); 
	tof8.push_back( 0x8000|(2+32) ); 
	tof8.push_back( 0x8000|(7+32) ); 
	tof8.push_back( 0x8000|(6+32) ); 
	tof8.push_back( 0x8000|(19+32)); 
	tof8.push_back( 0x8000|(18+32)); 
	tof8.push_back( 0x8000|(23+32)); 
	tof8.push_back( 0x8000|(22+32));
	
	m_sensorTypeTofChannelAssignment.insert(SensorTypes::TOF_1_TEMP, tof1);
	m_sensorTypeTofChannelAssignment.insert(SensorTypes::TOF_2_TEMP, tof2);
	m_sensorTypeTofChannelAssignment.insert(SensorTypes::TOF_3_TEMP, tof3);
	m_sensorTypeTofChannelAssignment.insert(SensorTypes::TOF_4_TEMP, tof4);
	m_sensorTypeTofChannelAssignment.insert(SensorTypes::TOF_5_TEMP, tof5);
	m_sensorTypeTofChannelAssignment.insert(SensorTypes::TOF_6_TEMP, tof6);
	m_sensorTypeTofChannelAssignment.insert(SensorTypes::TOF_7_TEMP, tof7);
	m_sensorTypeTofChannelAssignment.insert(SensorTypes::TOF_8_TEMP, tof8);
}

TOFSensorTypeAssignment::~TOFSensorTypeAssignment() {

}

SensorTypes::Type TOFSensorTypeAssignment::tofSensorType(unsigned int tofChannel) {
	bool foundChannel = false;
	foreach(SensorTypes::Type type, m_sensorTypeTofChannelAssignment.keys()) {
		if (m_sensorTypeTofChannelAssignment[type].contains(tofChannel)) {
			if (foundChannel == false) {
				foundChannel = true;
				return type;
			}
			else {
				qDebug("ERROR: Duplicated tof channel ids in the Assignment map for id: 0x%x", tofChannel);
			}
		}
	}
	qDebug("ERROR: Tof sensor type assignment does not contain this id: 0x%x", tofChannel);
	return SensorTypes::START;
}





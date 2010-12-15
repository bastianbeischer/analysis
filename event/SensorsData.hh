#ifndef SensorsData_hh
#define SensorsData_hh

#include <TFile.h>
#include <TTree.h>
#include <map>
#include <math.h>
#include <iostream>

class SensorsData {
public:
	enum DataType { SENSORS, ATC, EBASS };
	
public:
	SensorsData();
	bool setFile(const char* file);
	double getPrevious(DataType type, const char* id, time_t time, unsigned int &diff);
	double getNext(DataType type, const char* id, time_t time, unsigned int &diff);
	double getAverage(DataType type, const char* id, time_t time); // Will return NaN if out of bounds.
	std::map<unsigned int,double> getValues(DataType type, const char* id);
	
	
	char** getKeys(DataType type);
	
private:
	std::map<unsigned int,unsigned int> m_sensorstimes;
	std::map<unsigned int,unsigned int> m_atctimes;
	std::map<unsigned int,unsigned int> m_ebasstimes;
	TTree* m_sensorstree;
	TTree* m_atctree;
	TTree* m_ebasstree;
	TFile* m_file;
	bool m_init;
	
	TTree* getTree(DataType type);
	std::map<unsigned int, unsigned int> getMap(DataType type);
	
};

#endif /* SensorsData_hh */

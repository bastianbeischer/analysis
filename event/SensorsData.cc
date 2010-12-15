/*
 *  SensorsData.cc
 *  
 *
 *  Created by Jens Wienkenhöver on 15.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SensorsData.hh"

SensorsData::SensorsData() {
	m_file = NULL;
	m_sensorstree = NULL;
	m_ebasstree = NULL;
	m_atctree = NULL;
	m_init = false;
}


bool SensorsData::setFile(const char* file) {
	m_file = new TFile(file);
	if (m_file->IsZombie()) {
		delete m_file;
		m_file = NULL;
		return false;
	} 
	m_sensorstree = (TTree*)m_file->Get("SENSORS");
	m_atctree = (TTree*)m_file->Get("ATC");
	m_ebasstree = (TTree*)m_file->Get("EBASS");
	
	if (!(m_sensorstree == NULL | m_atctree == NULL | m_ebasstree == NULL)) {
		delete m_sensorstree;
		delete m_atctree;
		delete m_ebasstree;
		m_sensorstree = NULL;
		m_atctree = NULL;
		m_ebasstree = NULL;
		return false;
	}
	unsigned int atctime;
	unsigned int sensortime;
	unsigned int ebasstime;
	m_atctree->SetBranchAddress("time",&atctime);
	for (int i = 0;i<m_atctree->GetEntries();i++) {
		m_atctree->GetEntry(i);
		m_atctimes[atctime] = i;
	}
	m_sensorstree->SetBranchAddress("time",&sensortime);
	for (int i = 0;i<m_sensorstree->GetEntries();i++) {
		m_sensorstree->GetEntry(i);
		m_sensorstimes[sensortime] = i;
	}
	m_ebasstree->SetBranchAddress("time",&ebasstime);
	for (int i = 0;i<m_ebasstree->GetEntries();i++) {
		m_ebasstree->GetEntry(i);
		m_ebasstimes[ebasstime] = i;
	}
	m_init = true;
	return true;
}

double SensorsData::getPrevious(DataType type, const char* id, time_t time, unsigned int &diff) {
	TTree* tree = this->getTree(type);
	std::map<unsigned int,unsigned int> map = this->getMap(type);
	double var;
	if (!tree->GetBranch(id)) {
		std::cerr << "Branch " << id << "not found.";
		return sqrt(-1);
	}
	tree->SetBranchAddress(id,&var);
	std::map<unsigned int,unsigned int>::iterator it = map.find(time);
	if (it == map.end()) {
		it = map.lower_bound(time);
	}
	if (it == map.end()) {
		return sqrt(-1);
	} else {
		tree->GetEntry((*it).second);
		while ((var != var) && (it != map.begin())) {
			it--;
		}
		if (it == map.begin()) {
			return sqrt(-1);
		} else {
			if (diff) {
				diff = time-(*it).first;
			}
			return var;
		}
	}
}


double SensorsData::getNext(DataType type, const char* id, time_t time, unsigned int &diff) {
	TTree* tree = this->getTree(type);
	std::map<unsigned int,unsigned int> map = this->getMap(type);
	double var;
	if (!tree->GetBranch(id)) {
		std::cerr << "Branch " << id << " not found.";
		return sqrt(-1);
	}
	tree->SetBranchAddress(id,&var);
	std::map<unsigned int,unsigned int>::iterator it = map.find(time);
	if (it == map.end()) {
		it = map.upper_bound(time);
	}
	if (it == map.end()) {
		return sqrt(-1);
	} else {
		tree->GetEntry((*it).second);
		while ((var != var) && (it != map.end())) {
			it++;
		}
		if (it == map.end()) {
			return sqrt(-1);
		} else {
			if (diff) {
				diff = (*it).first - time;
			}
			return var;
		}
	}
}

double SensorsData::getAverage(DataType type, const char* id, time_t time) {
	unsigned int diff1, diff2;
	double first = this->getNext(type, id, time, diff1);
	double second = this->getPrevious(type, id, time, diff2);
	if (diff1-diff2) {
		return ((diff1*first + diff2*second))/((double)(diff1+diff2));
	} else {
		return first;
	}
}

std::map<unsigned int,double> SensorsData::getValues(DataType type, const char* id) {
	TTree* tree = getTree(type);
	std::map<unsigned int,double> map;
	double var;
	int time;
	if (!tree->GetBranch(id)) {
		std::cerr << "Branch " << id << " not found.";
		return map;
	}
	tree->SetBranchAddress(id,&var);
	tree->SetBranchAddress("time",&time);
	for (int i = 0; i<tree->GetEntries();i++) {
		tree->GetEntry(i);
		map[time] = var;
	}
	return map;
}

TTree* SensorsData::getTree(DataType type) {
	switch (type) {
		case ATC:
			return m_atctree;
			break;
		case EBASS:
			return m_ebasstree;
			break;
		case SENSORS:
			return m_sensorstree;
			break;
		default:
			return NULL;
			break;
	}
}

std::map<unsigned int,unsigned int> SensorsData::getMap(DataType type) {
	switch (type) {
		case ATC:
			return m_atctimes;
			break;
		case EBASS:
			return m_ebasstimes;
			break;
		case SENSORS:
			return m_sensorstimes;
			break;
		default:
			std::map<unsigned int, unsigned int> emptymap;
			return emptymap;
			break;
	}
}

char** SensorsData::getKeys(DataType type) {


}
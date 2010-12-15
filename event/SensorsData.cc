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
	m_sensorstree = (TTree*)m_file->Get("sensors");
	m_atctree = (TTree*)m_file->Get("ATC");
#ifdef EBASS
	m_ebasstree = (TTree*)m_file->Get("EBASS");
#endif EBASS
	if ((m_sensorstree == NULL | m_atctree == NULL /*| m_ebasstree == NULL*/)) {
		delete m_sensorstree;
		delete m_atctree;
#ifdef EBASS
		delete m_ebasstree;
#endif
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
#ifdef EBASS
	m_ebasstree->SetBranchAddress("time",&ebasstime);
	for (int i = 0;i<m_ebasstree->GetEntries();i++) {
		m_ebasstree->GetEntry(i);
		m_ebasstimes[ebasstime] = i;
	}
#endif
	m_init = true;
	return true;
}

float SensorsData::getPrevious(DataType type, const char* id, time_t time, unsigned int *diff) {
	TTree* tree = this->getTree(type);
	std::map<unsigned int,unsigned int> map = this->getMap(type);
	float var = sqrt(-1);
	if (!tree->GetBranch(id)) {
		std::cerr << "Branch " << id << "not found.";
		return sqrt(-1);
	}
	tree->SetBranchAddress(id,&var);
	std::map<unsigned int,unsigned int>::iterator it = map.find(time);
	if (it == map.end()) {
		it = map.lower_bound(time);
		if ((*it).first > time) {
			it--;
		};
	}
	if (it == map.end() || it == map.begin()) {
		return sqrt(-1);
	} else {
		tree->GetEntry((*it).second);
		while ((!(var == var)) && (it != map.begin())) {
			it--;
			tree->GetEntry((*it).second);
		}
		if (it == map.begin()) {
			return sqrt(-1);
		} else {
			*diff = time-(*it).first;
			return var;
		}
	}
	
}


float SensorsData::getNext(DataType type, const char* id, time_t time, unsigned int *diff) {
	TTree* tree = this->getTree(type);
	std::map<unsigned int,unsigned int> map = this->getMap(type);
	float var = sqrt(-1);
	if (!tree->GetBranch(id)) {
		std::cerr << "Branch " << id << " not found.";
		return sqrt(-1);
	}
	tree->SetBranchAddress(id,&var);
	std::map<unsigned int,unsigned int>::iterator it = map.find(time);
	if (it == map.end() || it == map.begin()) {
		it = map.lower_bound(time);
	}
	if (it == map.end()) {
		return sqrt(-1);
	} else {
		tree->GetEntry((*it).second);
		while ((!(var == var)) && (it != map.end())) {
			it++;
			tree->GetEntry((*it).second);
		}
		if (it == map.end()) {
			return sqrt(-1);
		} else {
			*diff = (*it).first - time;
			return var;
		}
	}
}

float SensorsData::getAverage(DataType type, const char* id, time_t time) {
	unsigned int diff1 = 0;
	unsigned int diff2 = 0;
	float first = this->getNext(type, id, time, &diff1);
	float second = this->getPrevious(type, id, time, &diff2);
	std::cout << "";
	if (diff2 == 0) {
		return second;
	} else {
		float value = (((float)(diff2)*first + (float)(diff1)*second))/((double)(diff1+diff2));
		value++;
		value--;
		return value;
	}
}

std::map<unsigned int,float> SensorsData::getValues(DataType type, const char* id) {
	TTree* tree = getTree(type);
	std::map<unsigned int,float> map;
	float var;
	unsigned int time;
	if (!tree->GetBranch(id)) {
		std::cerr << "Branch " << id << " not found.";
		return map;
	}
	tree->SetBranchAddress(id,&var);
	tree->SetBranchAddress("time",&time);
	for (int i = 0; i<tree->GetEntries();i++) {
		tree->GetEntry(i);
		if (var==var) {
			map[time] = var;
		}
	}
	return map;
}

TTree* SensorsData::getTree(DataType type) {
	switch (type) {
		case ATC:
			return m_atctree;
			break;
#ifdef EBASS
		case EBASS:
			return m_ebasstree;
			break;
#endif
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
#ifdef EBASS
		case EBASS:
			return m_ebasstimes;
			break;
#endif
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
	TTree* tree = getTree(type);
	TObjArray* array = tree->GetListOfBranches();
	char** array2 = new char*[array->GetEntries()];
	
	
	int i = 0;
	int k = 0;
	while (k<array->GetEntries()) {
		if (strcmp(array->At(i)->GetName(),"time") != 0) {
			array2[i] = const_cast<char*>(array->At(k)->GetName());
			i++;
		}
		k++;
	}
	return array2;
}

int SensorsData::getNumberOfKeys(DataType type) {
	TTree* tree = getTree(type);
	TObjArray* array = tree->GetListOfBranches();
	return array->GetEntries()-1;
}
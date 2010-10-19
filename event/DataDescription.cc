#include "DataDescription.hh"

#include <stdio.h>
#include <iostream>

ClassImp(DataDescription);

DataDescription::DataDescription()
{
	FILE* file = popen("git rev-parse HEAD", "r");
	std::cout << file;

	//m_softwareVersionHash = ;
}

DataDescription::~DataDescription()
{}


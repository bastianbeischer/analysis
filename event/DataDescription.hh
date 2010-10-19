#ifndef DataDescription_h
#define DataDescription_h

#include <TObject.h>

class DataDescription : public TObject
{
	public:
		DataDescription();
		~DataDescription();
	private:
		char m_softwareVersionHash[41]; // 40 chars + '\0', SHA1 hash of the latest commit



	ClassDef( DataDescription, 1 );
};

#endif

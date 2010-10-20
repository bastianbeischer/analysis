#ifndef millepede_h
#define millepede_h

extern "C" void initgl_( unsigned int& nagb, unsigned int& nalc, unsigned int& nstdev, int& iprlim );
#define INITGL initgl_

extern "C" void zerloc_( float* dergb, float* derlc );
#define ZERLOC zerloc_

extern "C" void equloc_( float* dergb, float* derlc, float& rmeas, float& sigma, unsigned int& hybrid, unsigned int& sipm );
#define EQULOC equloc_

extern "C" void fitloc_( );
#define FITLOC fitloc_

extern "C" void constf_( float* dercs, float& rhs );
#define CONSTF constf_

extern "C" void testmp_( unsigned int& iarg );
#define TESTMP testmp_

extern "C" void fitglo_( float* par );
#define FITGLO fitglo_

extern "C" void parsig_( unsigned int& i, float& value );
#define PARSIG parsig_

extern "C" void initun_( unsigned int& nIter, float& otherVal );
#define INITUN initun_

extern "C" void parglo_ ( float* par);
#define PARGLO parglo_

#endif /* millepede_h */


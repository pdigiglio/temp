
#ifndef  potts_INC
#define  potts_INC

/* base class */
#include "reticolo.h"

/*
 * ==================================================================
 *        Class: Potts
 *  Description: 
 * ==================================================================
 */
class Potts: public Reticolo {
	public:	
		Potts (void) {}; /* ctor */
		virtual ~Potts (void) {}; /* dtor */

		long int energy ( void );
		void sweep ( void );

	protected:
		signed short int delta_E ( const Sito *p, spin s_new );
		signed short int single_E ( unsigned int i, unsigned int j );
		
}; /* -----  end of class Potts  ----- */

#endif   /* ----- #ifndef potts_INC  ----- */


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
		Potts (void); /* ctor */
		virtual ~Potts (void) {}; /* dtor */

		long int energy ( void );

		/* metropolis system update */
		void sweep ( void );
		/* Swendsen-Wang multi-cluster algorithm */
		void Sweep ( void );

		const long double * get_Ms ( void );

	protected:
		/* possible values of magnetization */
		long double mag[Q][2];

		signed short int delta_E ( const Sito *p, spin s_new );
		signed short int single_E ( unsigned int i, unsigned int j );
		
		/* create one cluster */
		unsigned long int cluster ( unsigned int i, unsigned int j );

		/* magnetizazion coupple of values */
		long double Ms[2] = {};
}; /* -----  end of class Potts  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Potts
 *      Method: get_Ms
 * Description: return magnetization (sweep)
 * ------------------------------------------------------------------
 */
inline const long double *
Potts::get_Ms ( void ) {
	return Ms;
} /* -----  end of method Potts::get_Ms  ----- */

#endif   /* ----- #ifndef potts_INC  ----- */

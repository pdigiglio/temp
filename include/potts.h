
#ifndef  potts_INC
#define  potts_INC

/* base class */
#include "reticolo.h"

/* number of states ( 2 -> Ising; 3 -> Potts ) */
#define Q	2

/*
 * ==================================================================
 *        Class: Potts
 *  Description: 
 * ==================================================================
 */
class Potts: public Reticolo {
	public:	
		Potts (void); /* ctor */
		virtual ~Potts (void); /* dtor */

		long int energy ( void );

		/* metropolis system update */
		void sweep ( void );
		/* Swendsen-Wang multi-cluster algorithm */
		void Sweep ( void );

		const long double * get_Ms ( void );

		/* useful constants (not to be evaluated at every cycle) */
		const long double EB = (long double) 1 - expl( (long double) - B );

	protected:
		/* possible values of magnetization */
		long double mag[Q][2];

		signed short int delta_E ( const Sito *p, spin s_new );
		signed short int single_E ( unsigned int i, unsigned int j );
		
		/* create one cluster */
		unsigned long int cluster ( unsigned int i, unsigned int j );

		/* magnetizazion coupple of values */
		long double Ms[2] = {};

		inline spin rand_init_val ( void );

		/* update magnetizazion checking the spin in xptr */
		void update_Ms ( Sito *xptr );
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

/*
 * ------------------------------------------------------------------
 *       Class: Potts
 *      Method: update_Ms
 * Description: 
 * ------------------------------------------------------------------
 */
inline void
Potts::update_Ms ( Sito *xptr ) {
	/* temporary magnetization pointers */
	long double *a = Ms, *b = *( mag + ( *xptr ).s );

	*( a ) += *( b );
	*( ++a ) += *( ++ b);
} /* -----  end of method Potts::update_Ms  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Potts
 *      Method: rand_init_val
 * Description: 
 * ------------------------------------------------------------------
 */
inline spin
Potts::rand_init_val ( void ) {
	return (spin) ( rand() % (unsigned) Q ); // Potts or higher
} /* -----  end of method Potts::rand_init_val  ----- */
#endif   /* ----- #ifndef potts_INC  ----- */

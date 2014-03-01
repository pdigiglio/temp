
#ifndef  ising_INC
#define  ising_INC

/* base class */
#include "reticolo.h"

/*
 * ==================================================================
 *        Class: Ising
 *  Description: 
 * ==================================================================
 */
class Ising: public Reticolo {
	public:	
		Ising (void); /* ctor */
		virtual ~Ising (void); /* dtor */
		
		/* metropolis system update */
		void sweep ( void );
		/* Swendsen-Wang multi-cluster algorithm */
		void Sweep ( void );

		/* evaluate correlator */
		void correlator ( void );

		/* magnetizazions measured in various ways */
		double get_M2 ( void );
		double get_Mm ( void );
		double get_Ms ( void );

		/* useful constants (not to be evaluated at every cycle) */
		const long double EB = (long double) 1 - expl( (long double) - 2 * B );

	protected:
		/* current magnetization */
//		long unsigned int M2;

		/* magnetizations (max cluster size, sweep) */
		unsigned long int Mm = 0;
		long int Ms = 0;

		/* create one cluster */
		unsigned long int cluster ( unsigned int i, unsigned int j );

		/* evaluate magnetization running all over lattice */
		long int magnetization ( void );

		/* evaluate lattice energy */
		long int energy ( void );
		/* return energy of site (i,j) */
		short int single_E ( unsigned int i, unsigned int j );
}; /* -----  end of class Ising  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: get_M2
 * Description: get susceptivity (to be normalized by L2)
 * ------------------------------------------------------------------
 */
//inline double
//Ising::get_M2 ( void ) {
//	/* normalize magnetization (divide by volume) */
//	return (double) Ising::M2;
//} /* -----  end of method Ising::get_M2  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: get_Mm
 * Description: 
 * ------------------------------------------------------------------
 */
inline double
Ising::get_Mm ( void ) {
	return (double) Mm;
} /* -----  end of method Ising::get_Mm  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Ising
 *      Method: get_Ms
 * Description: 
 * ------------------------------------------------------------------
 */
inline double
Ising::get_Ms ( void ) {
	return (double) Ms;
} /* -----  end of method Ising::get_Ms  ----- */

#endif   /* ----- #ifndef ising_INC  ----- */

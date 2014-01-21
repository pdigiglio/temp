
#ifndef  reticolo_INC
#define  reticolo_INC

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "beta.h"

/* lattice side */
#define	L	500
/* dimension */
#define D 2

/* short-cut to extend do Potts model */
typedef short int spin;

/*
 * ==================================================================
 *        Class: Reticolo
 *  Description: 
 * ==================================================================
 */
class Reticolo {
	public:
		Reticolo (void); /* ctor */
		~Reticolo (void); /* dtor */

		double get_E ( void );
		double get_M ( void );

		/* metropolis system update */
		void sweep ( void );
		/* Swendsen-Wang algorithm */
		void Sweep ( void );

		void print_lattice ( void );

		/* return energy of site (i,j) */
		short int single_E ( unsigned int i, unsigned int j );
		/* evaluate lattice energy */
		long int energy ( void );

		/* useful constants (not to be evaluated at every cycle) */
		const long double EB = (long double) 1 - expl( -B );
		static const long int L2 = L * L;
		
	protected:
		struct sito {
			/* nearest neighbours */
			unsigned short int nn[2 * D][D];
			/* spin value */
			spin s;
		} x[L][L];

		typedef struct sito Sito;
//		typedef short int Sito;
//		Sito x[L][L];

		/* controls if a site has been already checked */
		bool ckd[L][L] = {};
		bool ckd_status = (bool) 0;
		
		/* auxiliary array to check nearest neighbours */
		const signed short int s[2 * D][D] = {
			{ 0, 1 },
			{ 1, 0 },
			{ 0, -1 },
			{ -1, 0 }
		};

		/* cluster sites stack */
		const unsigned int head = 0;
		unsigned int tail = 0;
		/* stack contains at most L * L elements */
		unsigned short int stack[L2][D];

		/* number of sweeps */
		unsigned int t = 0;

		/* energy initialized in ctor */
		long int E;
		/* current magnetization */
		long int M;

		/* returns spin in *pos */
		spin S ( const unsigned short int p[] );

		/* return spin */
//		bool spin ( unsigned int i, unsigned int j );
		
		void cluster ( unsigned int i, unsigned int j );
}; /* -----  end of class Reticolo  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: get_E
 * Description: get energy (divide by L2 to have density!)
 * ------------------------------------------------------------------
 */
inline double
Reticolo::get_E ( void ) {
	return (double) Reticolo::E;
} /* -----  end of method Reticolo::get_E  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: get_M
 * Description: get magnetization (to be normalized by L2)
 * ------------------------------------------------------------------
 */
inline double
Reticolo::get_M ( void ) {
	/* normalize magnetization (divide by volume) */
	return (double) Reticolo::M;
} /* -----  end of method Reticolo::get_M  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: S
 * Description: return spin of site in ( p[0], p[1] ).
 * ------------------------------------------------------------------
 */
inline spin
Reticolo::S ( const unsigned short int p[] ) {
	return (*( *( x + *p ) + *( ++ p ) )).s;
} /* -----  end of method Reticolo::S  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Reticolo
 *      Method: spin
 * Description: 
 * ------------------------------------------------------------------
 */
//inline bool
//Reticolo::spin ( unsigned int i, unsigned int j ) {
//	return (bool) ( *( *( x + i) + j / 16 ) & *( t + j % 16 ) );
//} /* -----  end of method Reticolo::spin  ----- */

#endif   /* ----- #ifndef reticolo_INC  ----- */

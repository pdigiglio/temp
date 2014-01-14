/* standard header */
#include <stdio.h>
#include <math.h>

/* dimension of the space (2- or 3-D) */
#include "dimensions.h"

/* number of particles per side */
#define N	4

#ifndef  particella_INC
#define  particella_INC

/*
 * ==================================================================
 *        Class: Particella
 *  Description: base class for system with more particles
 * ==================================================================
 */
class Particella {
	public:	
		Particella ( void ); /* ctor */
		virtual ~Particella (void); /* dtor */

		/* return d-th component of n-th particle */
		double get_velocity( unsigned int n, unsigned short d = 0 );
		/* return kinetic energy */
		double get_K ( void );
		/* return current value of 'tm' counter */
		double get_time ( void );
		/* return internal energy */
		double get_U ( void );
		/* return kT (obtained from kinetik energy */
		virtual double get_KT ( void );
		/* return current pressure value */
		double get_pr ( void );
		/* return square mean radius */
		double get_R2M ( void );

		/* reset pressure value to 'val' */
		void reset_pr ( double val = (double) 0 );
		/* reset R2M value to 'val' */
		void reset_R2M ( double val = (double) 0 );

		/* print particle coordinates */
		void print_x ( void );
		
		/* save particles coordinates in r */
		void save_coordinates ( void );

		/* numero massimo di particelle nel volume */
		const static unsigned long int nMax = (unsigned long) 2 * powl( N, D );
		/* numero di coppie di particelle */
		const static unsigned long int e = nMax * ( nMax - 1 ) / 2;

	protected:
		/* record che rappresenta una particella */
		struct ptcl {
			double x[D]; /* posizione della particella */
			double v[D]; /* velocità della particella */
			double a[D]; /* accelerazione della particella */
			double t_last = (double) 0;
		} p[ nMax ];// = NULL;

		double r[nMax][D] = {};
		/* mean square radius */
		double R2M = (double) 0;

		/* for portability */
//		typedef;

		/* energia cinetica totale (m = 1) */
		double K = (double) 0;
		/* energia interna */
		double U = (double) 0;

		/* pressure temporary variable */
		double pr = (double) 0;

		/* Scalar product between two vectors, a and b, in D dimensions */
		double sp ( const double *a, const double *b );
		
		/* system time, updated at each evolution */
		double tm = (double) 0;

		/*
		 * The same as above: if only 'a' is given, it returns his
		 * square modulus
		 */
		double sp ( const double *a );

		/* returns distance between particles 'i' and 'j' */
		double distance( unsigned int i, unsigned int j );
}; /* -----  end of class Particella  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: get_KT
 * Description: return KT evaluated from kinetic energy
 * ------------------------------------------------------------------
 */
inline double
Particella::get_KT ( void ) {
	return (double) 2 * K / ( D * nMax );
} /* -----  end of method Particella::get_KT  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: get_time
 * Description: return current system time
 * ------------------------------------------------------------------
 */
inline double
Particella::get_time ( void ) {
	return (double) tm;
} /* -----  end of method Particella::get_time  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: get_pr
 * Description: return current value of pressure
 * ------------------------------------------------------------------
 */
inline double
Particella::get_pr ( void ) {
	return (double) pr;
} /* -----  end of method Particella::get_pr  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: reset_pr
 * Description: reset pressure counter
 * ------------------------------------------------------------------
 */
inline void
Particella::reset_pr ( double val ) {
	pr = (double) val;
} /* -----  end of method Particella::reset_pr  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: get_K
 * Description: return kinetic energy
 * ------------------------------------------------------------------
 */
inline double
Particella::get_K ( void ) {
	return (double) K;
} /* -----  end of method Particella::get_K  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: get_U
 * Description: 
 * ------------------------------------------------------------------
 */
inline double
Particella::get_U ( void ) {
	return (double) U;
} /* -----  end of method Particella::get_U  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: get_velocity
 * Description: return d-th velocity component of n-th particle
 * ------------------------------------------------------------------
 */
inline double
Particella::get_velocity ( unsigned int n, unsigned short int d ) {
//	return sqrt( Particella::sp( (*( p + n )).v ) );
	return *( (*( p + n )). v + d );
} /* -----  end of method Particella::get_velocity  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: sp
 * Description: 
 * ------------------------------------------------------------------
 */
inline double
Particella::sp ( const double *a, const double *b ) {
	double tmp = (double) 0;
	for ( unsigned short int d = 0; d < D; d ++ )
		tmp += *( a + d ) * *( b + d );

	return tmp;
} /* -----  end of method Particella::sp  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: sp
 * Description: 
 * ------------------------------------------------------------------
 */
inline double
Particella::sp ( const double *a ) {
	return Particella::sp( a, a );
} /* -----  end of method Particella::sp  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: ~Particella
 * Description: 
 * ------------------------------------------------------------------
 */
inline
Particella::~Particella ( void ) {
	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET ": "
					ANSI_YELLOW "%s" ANSI_RESET "] destructed.\n", __func__ );
} /* -----  end of method Particella::~Particella  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: distance
 * Description: return distance between i-th and j-th particles
 * ------------------------------------------------------------------
 */
inline double
Particella::distance ( unsigned int i, unsigned int j ) {
	double r[D];
	for ( unsigned int d = 0; d < D; d ++ ) {
		*( r + d ) = *( (*(p + i)).x + d ) - *( (*(p + j)).x + d );
		*( r + d ) -= round( *( r + d ) );
	}

	return sqrt( Particella::sp( r ) );
} /* -----  end of method Particella::distance  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: get_R2M
 * Description: return square mean radius
 * ------------------------------------------------------------------
 */
inline double
Particella::get_R2M ( void ) {
	return (double) Particella::R2M / Particella::nMax;
} /* -----  end of method Particella::get_R2M  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Particella
 *      Method: reset_R2M
 * Description: 
 * ------------------------------------------------------------------
 */
inline void
Particella::reset_R2M ( double val ) {
	Particella::R2M = (double) val;
} /* -----  end of method Particella::reset_R2M  ----- */

#endif   /* ----- #ifndef particella_INC  ----- */

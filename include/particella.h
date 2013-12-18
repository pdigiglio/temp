/* standard header */
#include <stdio.h>
#include <math.h>

/* dimension of the space (2- or 3-D) */
#include "dimensions.h"

/* packing fraction */
#include "eta.h"

/* number of particles per side */
#define N	7

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
		/* return kT (obtained from kinetik energy */
		double get_KT ( void );

		/* print particle coordinates */
		void print_x ( void );

		/* numero massimo di particelle nel volume */
		const static unsigned long int nMax = (unsigned long) 2 * powl( N, D );

		/* 
		 * raggio delle sfere
		 *
		 * XXX Nel passaggio da 2-D e 3-D i coefficienti e le potenze
		 * cambiano (a causa del passaggio area -> volume).
		 */
//		const double S = (double) 2 * sqrt( (double) E / ( nMax * M_PI ) );
		const double S = pow( (double) 6 * E / ( nMax * M_PI ), (double) 1 / D );

	protected:
		/* record che rappresenta una particella */
		struct ptcl {
			double x[D]; /* posizione della particella */
			double v[D]; /* velocità della particella */
			double t_last = (double) 0;
		} p[ nMax ];// = NULL;

		/* energia cinetica totale (m = 1) */
		double K = (double) 0;

		/* Scalar product between two vectors, a and b, in D dimensions */
		double sp ( const double *a, const double *b );

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

#endif   /* ----- #ifndef particella_INC  ----- */

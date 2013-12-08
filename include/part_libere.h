/* standard headers */
#include <stdlib.h>
#include <math.h>

/* base class */
#include "particella.h"
/* packing fraction */
#include "eta.h"
/* dimension of the space (2- or 3-D) */
#include "dimensions.h"

/* number of particles per side */
#define N 4

#ifndef  part_libere_INC
#define  part_libere_INC

/*
 * ==================================================================
 *        Class: Sistema
 *  Description: 
 * ==================================================================
 */
class Sistema: public Particella {
	public:
		Sistema ( void ); /* ctor */
		~Sistema ( void ); /* dtor */

		double get_time ( void );
		double get_pr ( void );
		double get_velocity( unsigned int n, unsigned short d = 0 );
		double get_K ( void );
		double get_KT ( void );

		void reset_pr ( double val = (double) 0 );

		double evolve ( void );
		void pression (void);

		void time_reset ( void );
		/* print particle coordinates */
		void print_x ( void );
		/* print free path of particles to '*stream' */
		void print_fp( FILE *stream = stdout );
		/* print particle collision times to '*stream' */
		void print_ct( FILE *stream = stdout );

		void mct ( void );
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

		/* speed of center of mass system */
		void mass_center_speed ( void );
	private:
		double tm = (double) 0;
		long double tau[2] = {};

		/* colliding particles */
		unsigned int i0, j0;
		double ti0, tj0, li0, lj0;

		/* matrice dei tempi di collisione */
		double **ct = NULL;
		/* nomber of list entries */
		static const unsigned long int e = ( nMax ) * ( nMax - 1) / 2;
		/* sorted list of collision times */
		double *list[e];
		/* find a way to evaluate this array */
		unsigned int L = 0;
		unsigned int *c = NULL;
		
		/* energia cinetica totale (m = 1) */
		double K = (double) 0;
		double pr = (double) 0;

		unsigned int crash = 0;

		/* scalar product */
//		double sp ( const double *a, const double *b );
//		/* if only 'a' is given, returns his square modulus */
//		double sp ( const double *a );


		/* takes next crash */
		double next_crash ( void );

		/* returns distance between particles 'i' and 'j' */
		double distance( unsigned int i, unsigned int j );

		/* shell sort algorithm */
		void shell_sort ( void );

		/* exchange particle velocity along r_ij axis */
		void exchange ( /* unsigned int i = i0, unsigned int j = j0 */ );
		/* collision time of particle 'i' with particle 'j' */
		double crash_time ( unsigned int i, unsigned int j );

		void update_crash_times ( double t0 );
//	protected:
}; /* -----  end of class Sistema  ----- */


/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: print_ct
 * Description: 
 * ------------------------------------------------------------------
 */
inline void
Sistema::print_ct ( FILE *stream ) {
	fprintf( stream, "%.16g\n%.16g\n", ti0, tj0 );
} /* -----  end of method Sistema::print_ct  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: print_fp
 * Description: 
 * ------------------------------------------------------------------
 */
inline void
Sistema::print_fp ( FILE *stream ) {
	fprintf( stream, "%.16g\n%.16g\n", li0, lj0 );
} /* -----  end of method Sistema::print_fp  ----- */


/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: distance
 * Description: 
 * ------------------------------------------------------------------
 */
inline double
Sistema::distance ( unsigned int i, unsigned int j ) {
	double r[D];
	for ( unsigned int d = 0; d < D; d ++ ) {
		*( r + d ) = *( (*(p + i)).x + d ) - *( (*(p + j)).x + d );
		*( r + d ) -= round( *( r + d ) );
	}

	return sqrt( Particella::sp( r ) );
} /* -----  end of method Sistema::distance  ----- */

#endif   /* ----- #ifndef part_libere_INC  ----- */

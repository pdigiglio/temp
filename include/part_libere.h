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
#define N	5
/* time step to evaluate $\Delta r^2(t)$ */
#define EPS	.0038


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

		/* return current value of 'tm' counter */
		double get_time ( void );
		/* return current pressure value */
		double get_pr ( void );
		/* return d-th component of n-th particle */
		double get_velocity( unsigned int n, unsigned short d = 0 );
		/* return kinetic energy */
		double get_K ( void );
		/* return kT (obtained from kinetik energy */
		double get_KT ( void );

		/* reset pressure value to 'val' */
		void reset_pr ( double val = (double) 0 );
		/* re-scale time matrix and 'tm' counter */
		void reset_time ( void );

		void save_coordinates ( void );

		/* evolve system between collisions */
		double evolve ( void );

		/* print particle coordinates */
		void print_x ( void );
		/* print free path of particles to '*stream' */
		void print_fp( FILE *stream = stdout );
		/* print particle collision times to '*stream' */
		void print_ct( FILE *stream = stdout );

		/* speed of center of mass system */
		void mass_center_speed ( void );


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

	private:
		/* system time, updated at each evolution */
		double tm = (double) 0;
		/* time when coordinates where measured last */
		double t_e = (double) 0;

		/* colliding particles */
		unsigned int i0, j0;
		/* mean collision times and free mean path */
		double ti0, tj0, li0, lj0;

		/* collision times matrix */
		double *ct[nMax];
		double r[nMax][D] = {};

		/* energia cinetica totale (m = 1) */
		double K = (double) 0;

		/* pressure temporary variable */
		double pr = (double) 0;

		/* returns distance between particles 'i' and 'j' */
		double distance( unsigned int i, unsigned int j );

		/* exchange particle velocity along r_ij axis */
		void exchange ( /* unsigned int i = i0, unsigned int j = j0 */ );

		/* return next collision time */
		double next_crash ( void );
		/* evaluate colliding time of particle 'i' with particle 'j' */
		double crash_time ( unsigned int i, unsigned int j );
		/* update colliding times matrix */
		void update_crash_times ( double t0 );
}; /* -----  end of class Sistema  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: get_KT
 * Description: return KT evaluated from kinetic energy
 * ------------------------------------------------------------------
 */
inline double
Sistema::get_KT ( void ) {
	return (double) 2 * K / ( D * nMax );
} /* -----  end of method Sistema::get_KT  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: get_K
 * Description: return kinetic energy
 * ------------------------------------------------------------------
 */
inline double
Sistema::get_K ( void ) {
	return (double) K;
} /* -----  end of method Sistema::get_K  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: get_velocity
 * Description: return d-th velocity component of n-th particle
 * ------------------------------------------------------------------
 */
inline double
Sistema::get_velocity ( unsigned int n, unsigned short int d ) {
//	return sqrt( Particella::sp( (*( p + n )).v ) );
	return *( (*( p + n )). v + d );
} /* -----  end of method Sistema::get_velocity  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: get_pr
 * Description: return current value of pressure
 * ------------------------------------------------------------------
 */
inline double
Sistema::get_pr ( void ) {
	return pr;
} /* -----  end of method Sistema::get_pr  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: get_time
 * Description: return current system time
 * ------------------------------------------------------------------
 */
inline double
Sistema::get_time ( void ) {
	return (double) tm;
} /* -----  end of method Sistema::get_time  ----- */
/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: reset_pr
 * Description: reset pressure counter
 * ------------------------------------------------------------------
 */
inline void
Sistema::reset_pr ( double val ) {
	pr = (double) val;
} /* -----  end of method Sistema::reset_pr  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Sistema
 *      Method: print_ct
 * Description: print single particle collision time of i0-th and
 * 				j0-th particles to '*stream'
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
 * Description: as above with the free path of particles
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
 * Description: return distance between i-th and j-th particles
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

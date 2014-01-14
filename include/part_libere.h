/* standard headers */
#include <stdlib.h>
#include <math.h>

/* base class */
#include "particella.h"
#include "eta.h"

/* time step to evaluate $\Delta r^2(t)$ */
#define EPS	.006

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

		/* re-scale time matrix and 'tm' counter */
		void reset_time ( void );

		/* evolve system between collisions */
		double evolve ( void );

		/* print free path of particles to '*stream' */
		void print_fp( FILE *stream = stdout );
		/* print particle collision times to '*stream' */
		void print_ct( FILE *stream = stdout );

		/* speed of center of mass system */
		void mass_center_speed ( void );

		/* 
		 * raggio delle sfere
		 *
		 * XXX Nel passaggio da 2-D e 3-D i coefficienti e le potenze
		 * cambiano (a causa del passaggio area -> volume).
		 */
//		const double S = (double) 2 * sqrt( (double) E / ( nMax * M_PI ) );
		const double S = pow( (double) 6 * E / ( nMax * M_PI ), (double) 1 / D );

	private:
		/* time when coordinates where measured last */
		double t_e = (double) 0;

		/* colliding particles */
		unsigned int i0, j0;
		/* mean collision times and free mean path */
		double ti0, tj0, li0, lj0;

		/* collision times matrix */
		double *ct[nMax];

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

#endif   /* ----- #ifndef part_libere_INC  ----- */

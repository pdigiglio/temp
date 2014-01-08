#include "particella.h"
#include "rho.h"

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

/* time step */
#define dt	.001

#ifndef  part_soffici_INC
#define  part_soffici_INC

/*
 * ==================================================================
 *        Class: Soft
 *  Description: 
 * ==================================================================
 */
class Soft: public Particella {
	public:
		Soft (void); /* ctor */
		~Soft (void); /* dtor */

		double evolve ( void );
		/* evolve using Euler algorithm */
		void evolve_euler ( void );

		/* set energy rescaling velocities */
		void set_energy( double e );
		/* set kT rescaling velocities */
		void set_kT ( double kT );
		
		/* return kT (obtained from kinetik energy) */
		double get_KT ( void );

		/* box side */
		const double L = pow( (double) Particella::nMax / RHO, (double) 1 / D );

	private:
		/* critic distance */
		const double r_c = (double) 5 / 2;
		/* force shift */
		const double F_c = .039;
		/* potenzial shift */
		const double V_c = -.0163;

		/* return force between particles */
		double F ( double r );
		/* potenzial energy */
		double V ( double r );
		
		/* scale speed modulus such to '|v| * scale' */
		void scale_velocity ( double scale );
}; /* -----  end of class Soft  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: F
 * Description: force between particles whose distance is 'r'
 * ------------------------------------------------------------------
 */
inline double
Soft::F ( double r ) {
	if ( r > r_c )
		return (double) 0;


	/* ( Sigma / r ) ^ 3 */
	double R = (double) 1 / ( r * r * r );
	/* ( Sigma / r ) ^ 6 */
	R *= R;

	return (double) 24 * R * ( 2 * R - 1 ) / r + F_c;
} /* -----  end of method Soft::F  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: V
 * Description: 
 * ------------------------------------------------------------------
 */
inline double
Soft::V ( double r ) {
	if ( r > r_c )
		return (double) 0;


	/* ( 1 / r ) ^ 3 */
	double R = (double) 1 / ( r * r * r );
	/* ( 1 / r ) ^ 6 */
	R *= R;

	return (double) 4 * R * ( R - 1 ) - V_c - ( r - r_c ) * F_c;
} /* -----  end of method Soft::V  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: ~Soft
 * Description: 
 * ------------------------------------------------------------------
 */
inline
Soft::~Soft ( void ) {
	fprintf( stderr, "[" ANSI_BLUE "info" ANSI_RESET ": "
					ANSI_YELLOW "%s" ANSI_RESET "] destructed.\n", __func__ );
} /* -----  end of method Soft::~Soft  ----- */

/*
 * ------------------------------------------------------------------
 *       Class: Soft
 *      Method: get_KT
 * Description: 
 * ------------------------------------------------------------------
 */
inline double
Soft::get_KT ( void ) {
	return (double) 2 * Particella::K / D;
} /* -----  end of method Soft::get_KT  ----- */

#endif   /* ----- #ifndef part_soffici_INC  ----- */

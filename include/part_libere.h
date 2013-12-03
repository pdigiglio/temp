#define D 3		/* dimensioni del problema */
#define N 4	/* sqrt[D]( numero particelle / 2 ) */

#include <stdlib.h>
#include <math.h>

/* packing fraction */
#include "eta.h"

#ifndef  part_libere_INC
#define  part_libere_INC

/*
 * ==================================================================
 *        Class: Sistema
 *  Description: 
 * ==================================================================
 */
class Sistema {
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

		/* record che rappresenta una particella */
		struct ptcl {
			double x[3]; /* posizione della particella */
			double v[3]; /* velocità della particella */
//			unsigned int crash = 0;
//			double t[2] = {};
		} *p = NULL;

		double tm = (double) 0;
		long double tau[2] = {};

		/* colliding particles */
		unsigned int i0, j0;

		/* matrice dei tempi di collisione */
		double **ct = NULL;
		/* sorted list of collision times */
		double **list = NULL;

		/* energia cinetica totale (m = 1) */
		double K = (double) 0;
		double pr = (double) 0;

		unsigned int crash = 0;

		/* scalar product */
		double sp ( const double *a, const double *b );
		/* if only 'a' is given, returns his square modulus */
		double sp ( const double *a );


		/* takes next crash */
		double next_crash ( void );

		/* shell sort algorithm */
		void shell_sort ( double *a, unsigned long int n );

		/* exchange particle velocity along r_ij axis */
		void exchange ( /* unsigned int i = i0, unsigned int j = j0 */ );
		/* collision time of particle 'i' with particle 'j' */
		double crash_time ( unsigned int i, unsigned int j );

		void update_crash_times ( double t0 );
//	protected:
}; /* -----  end of class Sistema  ----- */

#endif   /* ----- #ifndef part_libere_INC  ----- */

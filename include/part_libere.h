#define D 3		/* dimensioni del problema */
#define N 4	/* sqrt[D]( numero particelle / 2 ) */
#define E .2	/* frazione d'impacchettamento $\eta$ */

//#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "TH1F.h"
#include "TCanvas.h"

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

		float get_velocity( unsigned int n, unsigned short d = 0 );

		void evolve ( void );
		void pression (void) ;

		void time_reset ( void );
		/* print particle coordinates */
		void print_x ( void );

		void mct ( void );
		/* numero massimo di particelle nel volume */
		const unsigned long int nMax = (unsigned long) 2 * powl( N, D );

	private:
		/* 
		 * raggio delle sfere
		 *
		 * XXX Nel passaggio da 2-D e 3-D i coefficienti e le potenze
		 * cambiano (a causa del passaggio area -> volume).
		 */
		const float S = pow( (float) 6 * E / ( nMax * M_PI ), (float) 1 / D );

		/* record che rappresenta una particella */
		struct ptcl {
			float x[3]; /* posizione della particella */
			float v[3]; /* velocità della particella */
//			unsigned int crash = 0;
		} *p = NULL;

		float tm = (float) 0;
		long double tau[2] = {};

		/* colliding particles */
		unsigned int i0, j0;

		/* matrice dei tempi di collisione */
		float **ct = NULL;

		/* energia cinetica totale (m = 1) */
		float K = (float) 0;
		float press[2] = {};

//		TCanvas *c = new TCanvas( "titolo" );
//		TH1F *histo = new TH1F( "histogram", "Scalar speed distribution (complessive)", 150, -3., 3. );

		unsigned int crash = 0;

		/* scalar product */
		float sp ( const float *a, const float *b );
		/* if only 'a' is given, returns his square modulus */
		float sp ( float *a );
		/* takes next crash */
		float next_crash ( void );

		/* exchange particle velocity along r_ij axis */
		void exchange ( /* unsigned int i = i0, unsigned int j = j0 */ );
		/* collision time of particle 'i' with particle 'j' */
		float crash_time ( unsigned int i, unsigned int j );
		/* Checks if particles can be contained in volume */
		void capacity_check ( void );
		/* speed of center of mass system */
		void mass_center_speed ( void );

		void update_crash_times ( float t0 );
//	protected:
}; /* -----  end of class Sistema  ----- */

#endif   /* ----- #ifndef part_libere_INC  ----- */

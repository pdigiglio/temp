#define D 3		/* dimensioni del problema */
#define N 10	/* sqrt[D]( numero particelle / 2 ) */
#define E .2	/* frazione d'impacchettamento $\eta$ */
//#define S .1	/* raggio particelle */

//#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
//		Sistema (const Sistema &other); /* copy ctor */
		~Sistema ( void ); /* dtor */

		/* assignment operator */
//		Sistema& operator = (const Sistema &other);
		void evolve ( void );

		void print_x ( void );
	private:
		/* numero massimo di particelle nel volume */
		const unsigned long int nMax = (unsigned long) 2 * powl( N, D );
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
		} *p = NULL;

		/* colliding particles */
		unsigned int i0 = 1, j0 = 0;
		/* matrice dei tempi di collisione */
		float **ct = NULL;

		/* energia cinetica totale (m = 1) */
		float K = (float) 0;

		/* scalar product */
		float sp ( const float *a, const float *b );
		/* if only 'a' is given, returns his square modulus */
		float sp ( float *a );
		/* takes next crash */
		float next_crash ( void );

		/* collision time of particle 'i' with particle 'j' */
		float crash_time ( unsigned int i, unsigned int j );
		/* Checks if particles can be contained in volume */
		void capacity_check ( void );
		/* speed of center of mass system */
		void mass_center_speed ( void );
//	protected:
}; /* -----  end of class Sistema  ----- */

#endif   /* ----- #ifndef part_libere_INC  ----- */
